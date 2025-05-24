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

class ProgessiveMerger: public ff::ff_node_t<OrderedArray,OrderedArray>
{
    public: 
        ProgessiveMerger(std::vector<OrderedArray>& datastream):datastream(datastream){}
        OrderedArray* svc(OrderedArray* in){
            for(auto& arr:datastream){
                ff_send_out(&arr);
            }
            return EOS;
        };
    private:
        std::vector<OrderedArray>& datastream;
};

bool MPI_Send_record(Record& r,int dest,int tag,MPI_Comm comm,size_t recordSize){
    int op_res;

    op_res=MPI_Send(&r.key,1,MPI_UNSIGNED_LONG,dest,tag,comm);
    if (op_res < 0) return false;
    
    op_res=MPI_Send(r.rpayload, recordSize,MPI_CHAR,dest,tag,comm);
    if (op_res < 0) return false;

    return true;
}

bool MPI_Recv_record(Record** r,int source,int tag,MPI_Comm comm,size_t recordSize){
    Record res;
    MPI_Status status;
    int op_res;
    op_res=MPI_Recv(&res.key,1,MPI_UNSIGNED_LONG,source,tag,comm,&status);
    if(op_res < 0) return false;
    res.rpayload=(char*)malloc(sizeof(char)*recordSize);
    op_res=MPI_Recv(res.rpayload,recordSize,MPI_CHAR,source,tag,comm,&status);
    if(op_res < 0) return false;

    *r=&res;

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
    
    MPI_Init(&argc,&argv);
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
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
        long nodeDataSize = std::ceil(dataVector.size()/(size-1));
        int dst_idx=1;
        for(int i=0;i<dataVector.size();i+=nodeDataSize){
            unsigned int start_idx=i;
            unsigned int stop_idx=i+std::min(nodeDataSize,(long)dataVector.size()-i);
            long size=stop_idx - start_idx;
            MPI_Send(&size,1,MPI_LONG,dst_idx,SPLIT_TAG,MPI_COMM_WORLD);
            //distribute data in a round robin fashion, no block-cyclic distribution
            for(int j=start_idx;j<stop_idx;j++){
                Record& r=dataVector[j];
                MPI_Send_record(r,dst_idx,SPLIT_TAG,MPI_COMM_WORLD,recordSize);
            }
            dst_idx=(dst_idx+1)%size;
        }

        for(int i=0;i<size;i++){
            MPI_Send(&STOP_SIGNAL,1,MPI_LONG,i,SPLIT_TAG,MPI_COMM_WORLD);
        }

        
        size_t stop_count=0;
        std::vector<OrderedArray> partialMerges;
        long data_buf_size;
        MPI_Status status;
        size_t finalSize=0;
        for(int i=1;i<size-1;i++){
            OrderedArray mergingDataVector;
            MPI_Recv(&data_buf_size,1,MPI_LONG,MPI_ANY_SOURCE,MERGE_TAG,MPI_COMM_WORLD,&status);
            int source = status.MPI_SOURCE;
            for(int i=0;i<data_buf_size;i++){
                Record* r;
                MPI_Recv_record(&r,source,MERGE_TAG,MPI_COMM_WORLD,recordSize);
                mergingDataVector.push_back(*r);
            }
            partialMerges.push_back(mergingDataVector);
            finalSize+=mergingDataVector.size();
        }
        std::cout << "] everything is received i'll start merging"<< std::endl;
        size_t numWorkers=std::max(1UL,numThreads/2);
        OrderedArray* result;

        ff::ff_farm merging_farm;
        merging_farm.add_emitter(MasterNode(finalSize,&result));
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
            for(int i=0;i<data_buf_size;i++){
                Record* r;
                MPI_Recv_record(&r,0,SPLIT_TAG,MPI_COMM_WORLD,recordSize);
                partialDataVector.push_back(*r);
            }
        }

        OrderedArray* result;
        //std::cout<< rank<<">start ordering t:" << numThreads << "ls:" << leafSize << std::endl;
        parallelFFMergeSort(&partialDataVector,&result,numThreads,leafSize);
        //std::cout<< rank<<">done ordering" << std::endl;
        long res_size=result->size();
        MPI_Send(&res_size,1,MPI_LONG,0,MERGE_TAG,MPI_COMM_WORLD);
        
        for(int j=0;j<result->size();j++){
            Record& r=result->at(j);
            MPI_Send_record(r,0,MERGE_TAG,MPI_COMM_WORLD,recordSize);
        }

        
    }
    MPI_Finalize();
    return 0;
}