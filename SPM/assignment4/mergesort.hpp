#include<vector>
#include <ff/ff.hpp>
#include <mpi.h>

#ifndef MERGESORT_HPP
#define MERGESORT_HPP
const unsigned int RPAYLOAD=1;

struct Record { 
    unsigned long key;  // sorting value 
    char rpayload[RPAYLOAD]; 
}; 

using OrderedArray=std::vector<Record>;
using task_t=std::pair<OrderedArray*,OrderedArray*>;

using PosKeyPair=std::pair<unsigned long,unsigned long>;
using PosKeyVec=std::vector<PosKeyPair>;
using Task_t=std::pair<PosKeyVec*,PosKeyVec*>;

void sequentialMerge(std::vector<Record>& arr, int left, int mid, int right);

void sequentialMergeSort(std::vector<Record>& arr, int left, int right);

struct MasterNode;

struct Worker;

std::unique_ptr<ff::ff_pipeline> spawn_ff_merging_pipeline(
    std::vector<Record>& src,
    PosKeyVec** dst,
    size_t numWorkers,
    size_t numSorters,
    size_t leafSize
);
std::vector<Record> spawn_datastream(size_t arraySize,size_t recordSize,unsigned int seed=42);

#endif