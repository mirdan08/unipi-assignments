#include<vector>

const size_t RPAYLOAD=100;

struct Record { 
    unsigned long key;  // sorting value 
    char rpayload[RPAYLOAD]; 
}; 

void sequentialMerge(std::vector<int>& arr, int left, int mid, int right);

void sequentialMergeSort(std::vector<int>& arr, int left, int right);