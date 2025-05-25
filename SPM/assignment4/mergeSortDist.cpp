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
    //r->rpayload=(char*)malloc(sizeof(char)*recordSize);
    op_res=MPI_Recv(r->rpayload,recordSize,MPI_CHAR,source,tag,comm,&status);
    if(op_res < 0) return false;

    return true;
}

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
    long nodeDataSize = std::ceil(arraySize/size);
    MPI_Datatype record_type=make_mpi_record_type();

    std::vector<Record> recvVector(nodeDataSize);
    OrderedArray* gatherVector=new OrderedArray(nodeDataSize*size);
    if(rank==0){
        std::cout << "size:" <<arraySize<<"\trecord size:" <<recordSize << "\tthreads:" <<numThreads << std::endl;
        if(verboseOutput){
            std::cout<< "ORIGINAL:" << std::endl;
            unsigned int i=1;
            for(const auto& v:sendVector){
                std::cout << "\t" << i++ << ":" << v.key << std::endl;
            }
        }
    }
    MPI_Request request;
    MPI_Iscatter(
        sendVector.data()   ,nodeDataSize   ,record_type,
        recvVector.data()   ,nodeDataSize   ,record_type,0,
        MPI_COMM_WORLD,
        &request
    );

    OrderedArray* res;
    parallelFFMergeSort(&recvVector,&res,numThreads,leafSize);

    MPI_Wait(&request,MPI_STATUS_IGNORE);
    MPI_Gather(
        res->data()         ,res->size()   ,record_type,
        gatherVector->data(),res->size()   ,record_type,0,
        MPI_COMM_WORLD
    );
    if(rank==0){
        OrderedArray* finalOrderedVector;
        parallelFFMergeSort(gatherVector,&finalOrderedVector,numThreads,leafSize);

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