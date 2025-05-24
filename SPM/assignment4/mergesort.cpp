//parallelized FastFlow version
#include<vector>
#include <ff/ff.hpp>
#include "mergesort.hpp"
#include<iostream>
#include<random>
#include<mpi.h>

class StreamParser: public ff::ff_node_t<OrderedArray,OrderedArray>
{
    public: 
        StreamParser(OrderedArray& datastream,size_t leafSize)
            :datastream(datastream),
            leafSize(leafSize){}

        OrderedArray* svc(OrderedArray* in){
            
            int n = datastream.size();
            for(int i=0;i<n;i+=leafSize){
                //start from base case and order if necessary
                size_t offset= (n-i>=leafSize)?leafSize: n-i;
                OrderedArray* partialRes=new OrderedArray(datastream.begin()+i,datastream.begin()+(i+offset));
                //send into the pipeline
                ff_send_out(partialRes);
            }
            return EOS;
        };
    private:
        OrderedArray& datastream;
        size_t leafSize;
};

struct MasterNode: public ff::ff_minode_t<OrderedArray,task_t>
{
    MasterNode(size_t stop_size,OrderedArray** result_ptr)
        :stop_size(stop_size),
         result_ptr(result_ptr){}
    public:
        task_t* svc(OrderedArray* in){
            assert(in->size()>0);
            //you received the last ordered array, stop
            if(in->size()==stop_size){
                *result_ptr=in;
                return EOS;
            }
            //buffer is empty add an element to it
            if(bufferedValue==nullptr){
                bufferedValue=in;
                return GO_ON;
            }
            task_t* res=new task_t(in,bufferedValue);
            ff_send_out(res);
            bufferedValue=nullptr;

            return GO_ON;

        }
    private:
        OrderedArray* bufferedValue=nullptr;
        OrderedArray** result_ptr=nullptr;
        size_t stop_size=0;
};

struct Worker: public ff::ff_node_t<task_t,OrderedArray>
{
    public:
        OrderedArray* svc(task_t* in){

            OrderedArray* res;
            OrderedArray* arr1=in->first;
            OrderedArray* arr2=in->second;

            if(arr1->at(arr1->size()-1).key<arr2->at(0).key){
                arr1->insert(arr1->end(),arr2->begin(),arr2->end());
                res=arr1;
                delete arr2;
            }
            else if(arr2->at(arr2->size()-1).key<arr1->at(0).key){
                arr2->insert(arr2->end(),arr1->begin(),arr1->end());
                res=arr2;
                delete arr1;
            }else{
                res=new OrderedArray();
                res->resize(arr1->size()+arr2->size());
                std::merge(
                    arr1->begin(),arr1->end(),
                    arr2->begin(),arr2->end(),
                    res->begin(),
                    [](Record a,Record b){return a.key< b.key;}
                );
                delete arr1;
                delete arr2;
            }

            ff_send_out(res);
            return GO_ON;
        }

};

struct SortingWorker: public ff::ff_monode_t<std::vector<Record>,OrderedArray>{
    OrderedArray* svc(std::vector<Record>* in){
        std::sort(
                    in->begin(),in->end(),
                    [](Record a,Record b){return a.key<b.key;}
                );
        return in;
    }
};

void parallelFFMergeSort(std::vector<Record>* src,OrderedArray** dst,size_t numThreads,size_t leafSize){
    ff::ff_farm merging_farm;

    ff::ff_farm sorting_farm;

    std::vector<ff::ff_node*> sorting_workers;

    size_t numSorters=std::max(1UL,numThreads/2);

    for(int i=0;i<numSorters;i++){
        sorting_workers.push_back(new SortingWorker());
    }
    sorting_farm.add_workers(sorting_workers);
    sorting_farm.remove_collector();


    size_t numWorkers=std::max(1UL,numThreads/2);
    merging_farm.add_emitter(MasterNode(src->size(),dst));
    
    std::vector<ff::ff_node*> farm_workers;
    for(int i=0;i<numWorkers;i++){
        farm_workers.push_back(new Worker());
    }
    merging_farm.add_workers(farm_workers);
    merging_farm.remove_collector();
    merging_farm.set_scheduling_ondemand();
    merging_farm.wrap_around();

    ff::ff_pipeline pipeline;



    StreamParser sp(*src,leafSize);
    pipeline.add_stage(sp);
    pipeline.add_stage(&sorting_farm);
    pipeline.add_stage(&merging_farm);

    if(pipeline.run_and_wait_end()<0){
        std::cerr << "something went wrong" << std::endl;
    }
}
std::vector<Record> spawn_datastream(size_t arraySize,size_t recordSize,unsigned int seed){
    std::mt19937 gen(seed); 

    std::uniform_int_distribution<> keyDis(0, 10000000);
    std::uniform_int_distribution<>  charDis(33, 126);


    std::vector<Record> dataStream(arraySize);
    std::transform(
        dataStream.begin(), dataStream.end(), 

        dataStream.begin(),
                [&](Record){ 
                    Record r;
                    r.key=keyDis(gen);
                    r.rpayload=(char*)malloc(sizeof(char)*recordSize);
                    for(int i=0;i<recordSize;i++){
                        r.rpayload[i]=charDis(gen);
                    }
                    return r;
                }
        );
    return dataStream;
}
