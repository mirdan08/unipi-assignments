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

class ProgessiveMerger: public ff::ff_node_t<PosKeyVec,PosKeyVec>
{
    public: 
        ProgessiveMerger(
            PosKeyVec& datastream,
            std::vector<int>& sendcounts,
            std::vector<int>& displs,
            int size

        )
            :datastream(datastream),
            sendcounts(sendcounts),
            displs(displs),
            size(size){}
        PosKeyVec* svc(PosKeyVec* in){
            for(int i=0;i<size;++i){
                ff_send_out(
                    new PosKeyVec(
                        datastream.begin()+displs[i],
                        datastream.begin()+displs[i]+sendcounts[i]
                    )
                );
            }
            return EOS;
        };
        private:
        std::vector<int>& sendcounts;
        std::vector<int>& displs;
        PosKeyVec& datastream;
        int size;
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

MPI_Datatype create_contiguous_poskeypair_type() {
    MPI_Datatype mpi_poskeypair;
    MPI_Type_contiguous(2, MPI_UNSIGNED_LONG, &mpi_poskeypair);  // 2 x unsigned long
    MPI_Type_commit(&mpi_poskeypair);
    return mpi_poskeypair;
}

struct MasterNode: public ff::ff_minode_t<PosKeyVec,Task_t>
{
    MasterNode(size_t stop_size,PosKeyVec** result_ptr)
        :stop_size(stop_size),
         result_ptr(result_ptr){}
    public:
        Task_t* svc(PosKeyVec* in){
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
            Task_t* res=new Task_t(in,bufferedValue);
            ff_send_out(res);
            bufferedValue=nullptr;

            return GO_ON;

        }
    private:
        PosKeyVec* bufferedValue=nullptr;
        PosKeyVec** result_ptr=nullptr;
        size_t stop_size=0;
};

inline void merge_sort_pairs(
    const PosKeyPair* __restrict__ A,
    size_t sizeA,
    const PosKeyPair* __restrict__ B,
    size_t sizeB,
    PosKeyPair* __restrict__ Out)
{
    size_t i = 0, j = 0, k = 0;

    // Merge both arrays until one is exhausted
    while (i < sizeA && j < sizeB) {
        // Inline comparison — this is usually cheaper than a ternary
        const int ak = A[i].second;
        const int bk = B[j].second;

        const bool takeA = ak <= bk;
        Out[k++] = takeA ? A[i++] : B[j++];
    }

    // Copy any remaining elements from A
    if (i < sizeA) {
        const size_t remaining = sizeA - i;
        std::memcpy(Out + k, A + i, remaining * sizeof(PosKeyPair));
        k += remaining;
    }

    // Copy any remaining elements from B
    if (j < sizeB) {
        const size_t remaining = sizeB - j;
        std::memcpy(Out + k, B + j, remaining * sizeof(PosKeyPair));
        k += remaining;
    }
}

struct Worker: public ff::ff_node_t<Task_t,PosKeyVec>
{
    public:
        PosKeyVec* svc(Task_t* in){
            PosKeyVec* res;
            PosKeyVec* arr1=in->first;
            PosKeyVec* arr2=in->second;
            res=new PosKeyVec(arr1->size()+arr2->size());
            merge_sort_pairs(arr1->data(),arr1->size(),arr2->data(),arr2->size(),res->data());
            ff_send_out(res);
            delete arr1;
            delete arr2;
            return GO_ON;
        }

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

    MPI_Datatype poskey_type=create_contiguous_poskeypair_type();
    
    std::vector<Record> recvVector(nodeDataSize);
    PosKeyVec gatherVector(nodeDataSize*size);
    std::vector<int> sendcounts(size);
    std::vector<int> displs(size);


    PosKeyVec* res;
    auto pipeline=spawn_ff_merging_pipeline(
        recvVector,&res,

        numThreads,numThreads,leafSize
    );
    
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
        sendVector.data(),sendcounts.data(),displs.data()  ,poskey_type,
        recvVector.data()   ,nodeDataSize   ,poskey_type,0,
        MPI_COMM_WORLD
    );
    pipeline->run_then_freeze();
    while(!pipeline->done()){}
    MPI_Gatherv(
        res->data()         ,res->size()   ,poskey_type,
        gatherVector.data(),sendcounts.data(),displs.data(),poskey_type,0,
        MPI_COMM_WORLD
    );
    if(rank==0){

        int flag=0;
        size_t recv_count=0;
        MPI_Status status;
        size_t numWorkers=numThreads;

        //building the FF parallel merger
        
        auto pipeline = std::make_unique<ff::ff_pipeline>();
        ProgessiveMerger pm(gatherVector,sendcounts,displs,size);
        pipeline->add_stage(&pm);

        auto merging_farm = std::make_unique<ff::ff_farm>();
        PosKeyVec* finalResult;
        auto master = new MasterNode(arraySize, &finalResult);
        merging_farm->add_emitter(master);
    
        std::vector<ff::ff_node*> farm_workers;
        for (int i = 0; i < numWorkers; ++i) {
            farm_workers.push_back(new Worker());
        }
        merging_farm->add_workers(farm_workers);
        merging_farm->remove_collector();
        merging_farm->set_scheduling_ondemand();
        merging_farm->wrap_around();
        pipeline->add_stage(merging_farm.release()); // pass ownership
        pipeline->run_then_freeze();
        while(!pipeline->done()){}

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        if(verboseOutput){
            std::cout<< "RESULT:" << std::endl;
            unsigned int i=1;
            for(const auto& v:(*finalResult)){
                std::cout << "\t" << i++ << ":" << v.second << std::endl;
            }
        }
        std::cout<< "time(ms):" << duration.count() << std::endl;
    }
    MPI_Finalize();
    return 0;
}