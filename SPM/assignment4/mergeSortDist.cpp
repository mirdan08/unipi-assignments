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

bool MPI_Send_record(Record& r,int dest,int tag,MPI_Comm comm,size_t recordSize){
    int op_res;

    op_res=MPI_Send(&r.key,1,MPI_UNSIGNED_LONG,dest,tag,comm);
    if (op_res < 0) return false;
    
    op_res=MPI_Send(r.rpayload, recordSize,MPI_CHAR,dest,tag,comm);
    if (op_res < 0) return false;

    return true;
}

bool MPI_Recv_record(Record* r,int source,int tag,MPI_Comm comm,size_t recordSize){
    MPI_Status status;
    int op_res;
    op_res=MPI_Recv(&r->key,1,MPI_UNSIGNED_LONG,source,tag,comm,&status);
    if(op_res < 0) return false;
    r->rpayload=(char*)malloc(sizeof(char)*recordSize);
    op_res=MPI_Recv(r->rpayload,recordSize,MPI_CHAR,source,tag,comm,&status);
    if(op_res < 0) return false;

    return true;
}

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
    MPI_Init(&argc,&argv);
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(size<2 && rank == 0){
        std::cout << "please allocate at least 2 nodes"<< std::endl;
    }

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
    //master node
    if(rank==0){
        auto startTime = std::chrono::high_resolution_clock::now();
        std::vector<Record> dataVector = spawn_datastream(arraySize,recordSize);
        std::cout << "size:" <<arraySize<<"\trecord size:" <<recordSize << "\tthreads:" <<numThreads << std::endl;
        if(verboseOutput){
            std::cout<< "ORIGINAL:" << std::endl;
            unsigned int i=1;
            for(const auto& v:dataVector){
                std::cout << "\t" << i++ << ":" << v.key << std::endl;
            }
        }
        long nodeDataSize = std::ceil(dataVector.size()/(size-1))+1;
        //std::cout << "]sending tasks out" << std::endl;
        unsigned int start_idx=0;
        unsigned int stop_idx=std::min(nodeDataSize,(long)dataVector.size());
        for(int dst_idx=1;dst_idx<size;dst_idx++){
            long taskSize=stop_idx - start_idx;
            
            //std::cout << "]sending taskSize " << taskSize << std::endl;
            MPI_Send(&taskSize,1,MPI_LONG,dst_idx,SPLIT_TAG,MPI_COMM_WORLD);
            //std::cout << "]sending data"<< std::endl;
            //statically distribute data to do computations
            for(int j=start_idx;j<stop_idx;j++){
                Record& r=dataVector[j];
                MPI_Send_record(r,dst_idx,SPLIT_TAG,MPI_COMM_WORLD,recordSize);
            }
            //std::cout << "]sent"<< std::endl;
            MPI_Send(&STOP_SIGNAL,1,MPI_LONG,dst_idx,SPLIT_TAG,MPI_COMM_WORLD);
            //std::cout << "]sent stop signal as well"<< std::endl;
            start_idx+= nodeDataSize;
            stop_idx+= std::min(nodeDataSize,(long)dataVector.size()-start_idx);
        }
        //std::cout << "]done sending tasks" << std::endl;

        
        size_t stop_count=0;
        std::vector<OrderedArray*> partialMerges;
        long data_buf_size;
        MPI_Status status;

        for(int i=1;i<size;i++){
            OrderedArray* mergingDataVector=new OrderedArray();
            MPI_Recv(&data_buf_size,1,MPI_LONG,MPI_ANY_SOURCE,MERGE_TAG,MPI_COMM_WORLD,&status);
            int source = status.MPI_SOURCE;
            for(int i=0;i<data_buf_size;i++){
                Record r;

                MPI_Recv_record(&r,source,MERGE_TAG,MPI_COMM_WORLD,recordSize);
                mergingDataVector->push_back(r);
            }
            partialMerges.push_back(mergingDataVector);
        }
        //std::cout<< "]gathered back the partial results" << std::endl;
        size_t numWorkers=std::max(1UL,numThreads/2);
        
        ff::ff_farm merging_farm;
        OrderedArray* result;
        merging_farm.add_emitter(MasterNode(arraySize,&result));
        std::vector<ff::ff_node*> farm_workers;
        for(int i=0;i<numWorkers;i++){
            farm_workers.push_back(new Worker());
        }
        merging_farm.add_workers(farm_workers);
        merging_farm.wrap_around();

        ff::ff_pipeline pipeline;


        ProgessiveMerger pm(partialMerges);

        pipeline.add_stage(pm);
        pipeline.add_stage(&merging_farm);
        //std::cout<< "]final merge starting" << std::endl;
        if(pipeline.run_and_wait_end()<0){
            std::cerr << "something went wrong" << std::endl;
        }
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

    }else{
        //all other nodes
        std::vector<Record> partialDataVector;
        long data_buf_size;
        MPI_Status status;
        while(true){
            MPI_Recv(&data_buf_size,1,MPI_LONG,0,SPLIT_TAG,MPI_COMM_WORLD,&status);
            if(data_buf_size==STOP_SIGNAL)
                break;
            //std::cout << rank <<">recevied "   << data_buf_size << std::endl;
            for(int i=0;i<data_buf_size;i++){
                Record r;
                MPI_Recv_record(&r,0,SPLIT_TAG,MPI_COMM_WORLD,recordSize);
                partialDataVector.push_back(r);
            }
        }

        OrderedArray* result;
        //std::cout<< rank<<">start ordering t:" << numThreads << " ls:" << leafSize << std::endl;
        parallelFFMergeSort(&partialDataVector,&result,numThreads,leafSize);
        //std::cout<< rank<<">done ordering" << std::endl;
        long res_size=result->size();
        MPI_Send(&res_size,1,MPI_LONG,0,MERGE_TAG,MPI_COMM_WORLD);
        
        for(int j=0;j<result->size();j++){
            Record& r=result->at(j);
            MPI_Send_record(r,0,MERGE_TAG,MPI_COMM_WORLD,recordSize);
        }
        
        //std::cout<< rank<<">done sending" << std::endl;
        
    }
    MPI_Finalize();
    return 0;
}