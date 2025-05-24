#include<vector>
#include <ff/ff.hpp>
#include <mpi.h>

struct Record { 
    unsigned long key;  // sorting value 
    char* rpayload; 
}; 

void sequentialMerge(std::vector<Record>& arr, int left, int mid, int right);

void sequentialMergeSort(std::vector<Record>& arr, int left, int right);

class StreamParser;

class MasterNode;

struct Worker;

using OrderedArray=std::vector<Record>;
using task_t=std::pair<OrderedArray*,OrderedArray*>;


void parallelFFMergeSort(std::vector<Record>* src,OrderedArray** dst,size_t numThreads,size_t leafSize);

std::vector<Record> spawn_datastream(size_t arraySize,size_t recordSize,unsigned int seed=42);