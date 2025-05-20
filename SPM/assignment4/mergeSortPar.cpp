
#include"mergesort.hpp"
#include<random>
#include<vector>
#include<iostream>
#include<algorithm>
#include <cstdlib>
#include <getopt.h>
#include <chrono>
#include <cstddef>
#include <vector>
#include <optional>
#include <memory>

#include <ff/ff.hpp>

//parallelized FastFlow version

using OrderedArray=std::vector<Record>;
using task_t=std::pair<OrderedArray*,OrderedArray*>;

//parsing arguments
size_t arraySize=100;
size_t recordSize = 100;
size_t leafSize = 11;
size_t numThreads = 8;
size_t numSorters=8;

class StreamParser: public ff::ff_node_t<OrderedArray,OrderedArray>
{
    public: 
        StreamParser(OrderedArray& datastream):datastream(datastream){}

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
    OrderedArray* svc(task_t* in){

        OrderedArray* res=new OrderedArray();

        OrderedArray* arr1=in->first;
        OrderedArray* arr2=in->second;
        res->reserve(arr1->size()+arr2->size());
        int i = 0, j = 0;
        while (i < arr1->size() && j < arr2->size()) {
            res->push_back(
                ( arr1->at(i).key <= arr2->at(j).key)?
                    arr1->at(i++)
                        :
                    arr2->at(j++)
            );
        }
        while (i < arr1->size()) res->push_back(arr1->at(i++));
        while (j < arr2->size()) res->push_back(arr2->at(j++));

        delete arr1;
        delete arr2;

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

void parallelFFMergeSort(std::vector<Record>* src,OrderedArray** dst){
    ff::ff_farm merging_farm;

    ff::ff_farm sorting_farm;

    std::vector<ff::ff_node*> sorting_workers;

    numSorters=std::max(1UL,numThreads/2);

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
    merging_farm.wrap_around();

    ff::ff_pipeline pipeline;



    StreamParser sp(*src);
    pipeline.add_stage(sp);
    pipeline.add_stage(&sorting_farm);
    pipeline.add_stage(&merging_farm);

    if(pipeline.run_and_wait_end()<0){
        std::cerr << "something went wrong" << std::endl;
    }
}

struct MergingFarm
{
    /* data */
};


int main(int argc,char*argv[]){

    bool verboseOutput=false;

    int opt;

    while ((opt = getopt(argc, argv, "s:r:t:v:l:")) != -1) {

        switch (opt) {
            case 's': {

                arraySize = std::stoull(optarg);
                break;
            }
            case 'r': {

                recordSize = std::stoull(optarg);
                break;
            }
            case 't': {

                numThreads = std::stoi(optarg);
                break;
            }
            case 'l':{
                leafSize = std::stoi(optarg);
                break;
            }
            case 'v':{
                int v = std::stoi(optarg);
                verboseOutput= (v>0?true:false);
                break;
            }
            default:{

                std::cout << "wrong arguments" << std::endl;
                return EXIT_FAILURE;
            }
        }
    }
    std::cout << "size:" <<arraySize<<"\trecord size:" <<recordSize << "\tthreads:" <<numThreads << std::endl;

    unsigned int seed=42;
    std::mt19937 gen(seed); 

    std::uniform_int_distribution<> keyDis(0, 10000000);
    std::uniform_int_distribution<>  charDis(33, 126);


    std::vector<Record> dataVector(arraySize);
    std::transform(
        dataVector.begin(), dataVector.end(), 

        dataVector.begin(),
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
    if(verboseOutput){
        std::cout<< "ORIGINAL:" << std::endl;
        unsigned int i=1;
        for(const auto& v:dataVector){
            std::cout << "\t" << i++ << ":" << v.key << std::endl;
        }
    }

    auto startTime = std::chrono::high_resolution_clock::now();
    OrderedArray* result;
    parallelFFMergeSort(&dataVector,&result);
    auto endTime = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    if(verboseOutput){
        std::cout<< "RESULT:" << std::endl;
        unsigned int i=1;
        for(const auto& v:(*result)){
            std::cout << "\t" << i++ << ":" << v.key << std::endl;
        }
    }
    std::cout<< "time(ms):" << duration.count() << std::endl;
}