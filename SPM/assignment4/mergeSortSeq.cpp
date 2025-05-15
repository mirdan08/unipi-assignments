
#include"mergesort.hpp"
#include<random>
#include<vector>
#include<iostream>
#include<algorithm>
#include <cstdlib>
#include <getopt.h>
#include <chrono>

int main(int argc,char*argv[]){

    //parsing arguments
    size_t arraySize=100;
    size_t recordSize = 0;
    int numThreads = 0;
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
    unsigned int seed=42;
    std::mt19937 gen(seed); 
    std::uniform_int_distribution<> dis(0, 100);
    std::vector<int> dataVector(arraySize);
    std::transform(dataVector.begin(), dataVector.end(), dataVector.begin(),
                   [&](int){ return dis(gen);}
            );
    if(verboseOutput){
        std::cout<< "ORIGINAL:" << std::endl;
        unsigned int i=1;
        for(const auto& v:dataVector){
            std::cout << i++ << ":" << v << std::endl;
        }
    }
    auto startTime = std::chrono::high_resolution_clock::now();
    sequentialMergeSort(dataVector,0,dataVector.size());
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout<< "time(ms):" << duration.count() << std::endl;
    if(verboseOutput){
        std::cout<< "RESULT:" << std::endl;
        unsigned int i=1;
        for(const auto& v:dataVector){
            std::cout << i++ << ":" << v << std::endl;
        }
    }
}