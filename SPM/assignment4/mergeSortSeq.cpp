
#include"mergesort.hpp"
#include<random>
#include<vector>
#include<iostream>
#include<algorithm>
#include <cstdlib>
#include <getopt.h>
#include <chrono>

int main(char*argv[],int argc){

    size_t arraySize=100;
    size_t recordSize = 0;
    int numThreads = 0;
    bool verboseOutput=false;
    int opt;
    while ((opt = getopt(argc, argv, "s:r:t:v:")) != -1) {
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
            default:
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
            
    auto startTime = std::chrono::high_resolution_clock::now();
    sequentialMergeSort(dataVector,0,dataVector.size());
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout<< "time(s):" << duration.count() << std::endl;
    if(verboseOutput){
        unsigned int i=1;
        for(const auto& v:dataVector){
            std::cout << i++ << ":" << v << std::endl;
        }
    }
}