
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

class StreamParser: public ff::ff_node_t<OrderedArray,OrderedArray>
{
    public: 
        StreamParser(OrderedArray& datastream):datastream(datastream){}

        OrderedArray* svc(OrderedArray* in){
            
            int n = datastream.size();
            for(int i=0;i<n;i+=2){
                OrderedArray* partialRes=new OrderedArray();
                //start from base case and order if necessary
                if(n-i>=2){
                    if(datastream[i].key<=datastream[i+1].key){
                        partialRes->push_back(datastream[i]);
                        partialRes->push_back(datastream[i+1]);
                    }else{
                        partialRes->push_back(datastream[i+1]);
                        partialRes->push_back(datastream[i]);
                    }
                }else{
                    //in case less then 2 elements are remaining
                    partialRes->push_back(datastream[i]);
                }
                //send into the pipeline
                ff_send_out(partialRes);
            }
            std::cout <<"end of stream parser data" << std::endl;
            return EOS;
        };
    private:
        OrderedArray& datastream;
};

struct BufferNode: public ff::ff_node_t<OrderedArray,task_t>
{
    BufferNode(size_t stop_size,OrderedArray** result_ptr)
        :stop_size(stop_size){
            this->result_ptr=result_ptr;
        }
    public:
        task_t* svc(OrderedArray* in){
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
        OrderedArray* bufferedValue;
        OrderedArray** result_ptr;
        size_t stop_size=0;
};

struct Worker: public ff::ff_node_t<task_t,OrderedArray>
{
    OrderedArray* svc(task_t* in){
        OrderedArray* res=new OrderedArray();

        
        OrderedArray* arr1=in->first;
        OrderedArray* arr2=in->second;
        int i = 0, j = 0, k = 0;
        while (i < arr1->size() && j < arr2->size()) {
            Record r1= arr1->at(i);
            Record r2= arr2->at(j);

            if ( r1.key <= r2.key)
                res->push_back(arr1->at(i++));
            else
                res->push_back(arr2->at(j++));
        }
        while (i < arr1->size()) res->push_back(arr1->at(i++));
        while (j < arr2->size()) res->push_back(arr2->at(j++));
        ff_send_out(res);
        return GO_ON;
    }
};

int main(int argc,char*argv[]){

    //parsing arguments
    size_t arraySize=100;
    size_t recordSize = 100;
    int numThreads = 8;
    bool verboseOutput=false;

    int opt;

    while ((opt = getopt(argc, argv, "s:r:t:v")) != -1) {

        switch (opt) {
            case 's':
                arraySize = std::stoull(optarg);
                break;
            case 'r':
                recordSize = std::stoull(optarg);
                break;
            case 't':
                numThreads = std::stoi(optarg);
                break;
            case 'v':
                verboseOutput=true;
                break;
            default:
            std::cout << "wrong arguments" << std::endl;
            return EXIT_FAILURE;
        }
    }

    std::cout << "size:" <<arraySize<<"\trecord size:" <<recordSize << "\tthreads:" <<numThreads << std::endl;

    unsigned int seed=42;
    std::mt19937 gen(seed); 

    std::uniform_int_distribution<> keyDis(0, 100);
    std::uniform_int_distribution<>  charDis(33, 126);


    std::vector<Record> dataVector(arraySize);
    std::transform(dataVector.begin(), dataVector.end(), dataVector.begin(),
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
            std::cout << i++ << ":" << v.key << std::endl;
        }
    }

    auto startTime = std::chrono::high_resolution_clock::now();
    ff::ff_farm merging_farm;

    size_t numWorkers=numThreads;

    OrderedArray* result;
    merging_farm.add_emitter(BufferNode(dataVector.size(),&result));
    
    std::vector<ff::ff_node*> farm_workers;
    for(int i=0;i<numWorkers;i++){
        farm_workers.push_back(new Worker());
    }
    merging_farm.add_workers(farm_workers);
    merging_farm.wrap_around();

    ff::ff_pipeline pipeline;
    StreamParser sp(dataVector);
    pipeline.add_stage(sp);
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
            std::cout << i++ << ":" << v.key << std::endl;
        }
    }
    std::cout<< "time(ms):" << duration.count() << std::endl;
}