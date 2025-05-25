
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
    while ((opt = getopt(argc, argv, "s:r:v:")) != -1) {

        switch (opt) {
            case 's':
                arraySize = std::stoull(optarg);
                break;
            case 'r':
                recordSize = std::stoull(optarg);
                break;
                case 'v':
                verboseOutput= std::stoull(optarg)==1 ? true : false;

                break;
            default:
            std::cout << "wrong arguments" << std::endl;
            return EXIT_FAILURE;
        }
    }
    unsigned int seed=42;
    std::mt19937 gen(seed); 

    std::uniform_int_distribution<> keyDis(0, 10000000);
    std::uniform_int_distribution<>  charDis(33, 126);


    std::vector<Record> dataVector(arraySize);
    std::transform(
        dataVector.begin(), dataVector.end(), 

        dataVector.begin(),
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
    std::sort(
        dataVector.begin(),dataVector.end(),
        [](Record a,Record b){return a.key<b.key;}
    );
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    if(verboseOutput){
        std::cout<< "RESULT:" << std::endl;
        unsigned int i=1;
        for(const auto& v:dataVector){
            std::cout << i++ << ":" << v.key << std::endl;
        }
    }
    std::cout<< "time(ms):" << duration.count() << std::endl;
}