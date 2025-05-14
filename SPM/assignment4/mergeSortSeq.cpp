
#include"mergesort.hpp"
#include<random>
#include<vector>
#include<iostream>
#include<algorithm>
#include <cstdlib>
#include <getopt.h>

int main(char*argv[],int argc){

    size_t arraySize=100;
    size_t recordSize = 0;
    int numThreads = 0;
    int opt;
    while ((opt = getopt(argc, argv, "s:r:t:")) != -1) {
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
    
    

    
    

}