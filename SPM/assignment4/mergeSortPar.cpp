
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

int main(int argc,char*argv[]){
    size_t arraySize=100;
    size_t recordSize=100;
    size_t leafSize=100;
    size_t numThreads=1;

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
    
    std::vector<Record> dataVector=spawn_datastream(arraySize,recordSize);

    if(verboseOutput){
        std::cout<< "ORIGINAL:" << std::endl;
        unsigned int i=1;
        for(const auto& v:dataVector){
            std::cout << "\t" << i++ << ":" << v.key << std::endl;
        }
    }

    auto startTime = std::chrono::high_resolution_clock::now();
    PosKeyVec* result;
    
    auto pipeline=spawn_ff_merging_pipeline(dataVector,&result,numThreads,numThreads,leafSize);
    pipeline->run_then_freeze();
    while(!pipeline->done()){}
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    if(verboseOutput){
        std::cout<< "RESULT:" << std::endl;
        unsigned int i=1;
        for(const auto& v:(*result)){
            std::cout << "\t" << i++ << ":" << v.second << std::endl;
        }
    }
    std::cout<< "time(ms):" << duration.count() << std::endl;
}