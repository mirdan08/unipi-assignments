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
#include <mpi.h>
#include <ff/ff.hpp>
#include <cassert>
#include <mutex>
#include <condition_variable>

struct SortingWorker: public ff::ff_monode_t<std::vector<Record>,OrderedArray>{
    OrderedArray* svc(std::vector<Record>* in){
        std::sort(
                    in->begin(),in->end(),
                    [](Record a,Record b){return a.key<b.key;}
                );
        return in;
    }
};



struct MasterNode: public ff::ff_minode_t<OrderedArray,task_t>
{
    MasterNode(size_t stop_size,OrderedArray** result_ptr)
        :stop_size(stop_size),
         result_ptr(result_ptr){}
    public:
        task_t* svc(OrderedArray* in){
            assert(in->size()>0);
            if(in->size()==1){
                std::cout<< "remaining" << std::endl;
            }
            //you received the last ordered array, stop
            if(in->size()==stop_size){
                *result_ptr=in;
                return EOS;
            }
            //buffer is empty add an element to it
            if(bufferedValue==nullptr){
                bufferedValue=in;
                //std::cout << "MN]buff size - " << in->size() << std::endl;
                return GO_ON;
            }
            task_t* res=new task_t(in,bufferedValue);
            
            
            //std::cout << "MN] task size " << in->size() << ":" << bufferedValue->size()  << std::endl;
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

            OrderedArray* res=nullptr;
            OrderedArray* arr1=in->first;
            OrderedArray* arr2=in->second;
            //small optimization in case the two are already ordered
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
class ProgessiveMerger: public ff::ff_node_t<OrderedArray,OrderedArray>
{
    public: 
        ProgessiveMerger(std::vector<OrderedArray*>& datastream):datastream(datastream){}
        OrderedArray* svc(OrderedArray* in){
            for(auto& arr:datastream){
                ff_send_out(arr);
            }
            return EOS;
        };
    private:
        std::vector<OrderedArray*>& datastream;
};

MPI_Datatype make_mpi_record_type() {
        MPI_Datatype record_type;
    
        Record dummy;
        MPI_Aint base, displs[2];
        int block_lengths[2] = {1, RPAYLOAD};
        MPI_Datatype types[2] = {MPI_UNSIGNED_LONG, MPI_CHAR};
    
        MPI_Get_address(&dummy, &base);
        MPI_Get_address(&dummy.key, &displs[0]);
        MPI_Get_address(&dummy.rpayload, &displs[1]);
    
        displs[0] -= base;
        displs[1] -= base;
    
        MPI_Type_create_struct(2, block_lengths, displs, types, &record_type);
        MPI_Type_commit(&record_type);
    
        return record_type;
}


class DynamicBufferNode : public ff::ff_node_t<OrderedArray,OrderedArray> {
    public:
    DynamicBufferNode(size_t stop_size):stop_size(stop_size),acc_size(0){}
        // Called from outside, thread-safe
        void add_data(const OrderedArray& data) {
            {
                std::lock_guard<std::mutex> lk(mtx);
                buffer.push(data);
            }
            cv.notify_one();
        }
    
        OrderedArray* svc(OrderedArray *) override {
            while (true) {

                std::unique_lock<std::mutex> lk(mtx);
                cv.wait(lk, [this]() { return !buffer.empty() || (stop_size<acc_size); });
                auto data = buffer.front();
                acc_size+=data.size();
                buffer.pop();
                lk.unlock();

                auto* data_ptr = new OrderedArray(std::move(data));
                ff_send_out(data_ptr);
                if (acc_size==stop_size || buffer.empty()) {
                    break;
                }

            }
            return EOS;
        }
    private:
        std::mutex mtx;
        std::condition_variable cv;
        std::queue<OrderedArray> buffer;
        DynamicBufferNode();
        size_t stop_size;
        size_t acc_size=0;
};

int main(int argc,char*argv[]){
    size_t arraySize=100;
    size_t recordSize=100;
    size_t leafSize=100;
    size_t numThreads=1;
    size_t numSorters=100;
    
    const int SPLIT_TAG=1;
    const int MERGE_TAG=2;
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
                numThreads = std::stoull(optarg);
                break;
            }
            case 'l':{
                leafSize = std::stoull(optarg);
                break;
            }
            case 'v':{
                int v = std::stoull(optarg);
                verboseOutput= (v>0?true:false);
                break;
            }
            default:{
                
                std::cout << "wrong arguments" << std::endl;
                return EXIT_FAILURE;
            }
        }
    }
    const long STOP_SIGNAL=-1;
    
    int rank,size;
    std::vector<Record> sendVector= spawn_datastream(arraySize,recordSize);


    
    auto startTime = std::chrono::high_resolution_clock::now();
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    size_t nodeDataSize = std::ceil(arraySize/size);
    MPI_Datatype record_type=make_mpi_record_type();

    std::vector<Record> recvVector(nodeDataSize);
    OrderedArray* gatherVector=new OrderedArray(nodeDataSize*size);
    std::vector<int> sendcounts(size);
    std::vector<int> displs(size);
    if(rank==0){
        std::cout << "size:" <<arraySize<<"\trecord size:" <<recordSize << "\tthreads:" <<numThreads << std::endl;
        if(verboseOutput){
            std::cout<< "ORIGINAL:" << std::endl;
            unsigned int i=1;
            for(const auto& v:sendVector){
                std::cout << "\t" << i++ << ":" << v.key << std::endl;
            }
        }



        displs[0] = 0;
        sendcounts[0]= std::min(nodeDataSize,sendVector.size());
        for (int i = 1; i < size; ++i) {
            sendcounts[i]=std::min(nodeDataSize, arraySize- i*nodeDataSize);
            displs[i] = displs[i - 1] + sendcounts[i - 1];
        }
    }
    MPI_Request request;




    MPI_Scatterv(
        sendVector.data(),sendcounts.data(),displs.data()  ,record_type,
        recvVector.data()   ,nodeDataSize   ,record_type,0,
        MPI_COMM_WORLD
    );


    OrderedArray* res;
    parallelFFMergeSort(&recvVector,&res,numThreads,leafSize);
    std::cout << rank << ">done ordering my portion" << std::endl;
    MPI_Gatherv(
        res->data()         ,res->size()   ,record_type,
        gatherVector->data(),sendcounts.data(),displs.data(),record_type,0,
        MPI_COMM_WORLD
    );

    if(rank==0){

        int flag=0;
        size_t recv_count=0;
        MPI_Status status;
        std::vector<OrderedArray*> partialRes;
        for(int i=0;i<size;i++){
            partialRes.push_back(
                new OrderedArray(
                    gatherVector->begin()+displs[i],
                    gatherVector->begin()+displs[i]+sendcounts[i]
                )
            );
        }
        //building the FF parallel merger
        ProgessiveMerger pm(partialRes);


        ff::ff_farm merging_farm;
        OrderedArray* finalOrderedVector;
        merging_farm.add_emitter(MasterNode(arraySize,&finalOrderedVector));
        size_t numWorkers=numThreads;
        std::vector<ff::ff_node*> farm_workers;
        for(int i=0;i<numWorkers;i++){
            farm_workers.push_back(new Worker());
        }
        merging_farm.add_workers(farm_workers);
        merging_farm.remove_collector();
        merging_farm.set_scheduling_ondemand();
        merging_farm.wrap_around();
        ff::ff_pipeline pipeline;

        pipeline.add_stage(&pm);
        pipeline.add_stage(&merging_farm);
        if(pipeline.run_and_wait_end()<0){
            std::cerr << "something went wrong" << std::endl;
        }

        auto endTime = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        if(verboseOutput){
            std::cout<< "RESULT:" << std::endl;
            unsigned int i=1;
            for(const auto& v:(*finalOrderedVector)){
                std::cout << "\t" << i++ << ":" << v.key << std::endl;
            }
        }
        std::cout<< "time(ms):" << duration.count() << std::endl;
    }
    MPI_Finalize();
    return 0;
}