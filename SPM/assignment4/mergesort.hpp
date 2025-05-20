#include<vector>
#include <ff/ff.hpp>

//const size_t RPAYLOAD=100;

struct Record { 
    unsigned long key;  // sorting value 
    char* rpayload; 
}; 

void sequentialMerge(std::vector<Record>& arr, int left, int mid, int right);

void sequentialMergeSort(std::vector<Record>& arr, int left, int right);