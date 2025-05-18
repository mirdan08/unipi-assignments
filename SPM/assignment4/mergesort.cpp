#include <cstddef>
#include <vector>
#include <optional>
#include <memory>
#include "mergesort.hpp"



//utility functions

//sequenmtial merge sort implementation
void sequentialMerge(std::vector<int>& arr, int left, int mid, int right) {
    // Create temporary arrays
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1), R(n2);

    // Copy data to temp arrays#include <memory>
    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    // Merge the temp arrays
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    // Copy any remaining elements
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void sequentialMergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        sequentialMergeSort(arr, left, mid);       // Sort left half
        sequentialMergeSort(arr, mid + 1, right);  // Sort right half
        sequentialMerge(arr, left, mid, right);    // Merge sorted halves
    }
}
//parallelized FastFlow version

using OrderedArray=std::vector<Record>;
using task_t=std::pair<OrderedArray,OrderedArray>;

class StreamParser: ff::ff_node_t<OrderedArray,OrderedArray>
{
    OrderedArray* svc(OrderedArray* in){
        int n = (*in).size();
        for(int i=0;i<n;i+=2){
            Record r;
            OrderedArray partialRes;
            //start from base case and order if necessary
            if(i!=n-1){
                if((*in)[i].key>(*in)[i+1].key){
                    r=(*in)[i];
                    in[i]=in[i+1];
                    (*in)[i+1]=r;
                }
                partialRes[0]=(*in)[i];
                partialRes[1]=(*in)[i+1];
            }else{
                partialRes[0]=(*in)[i];
            }
            auto task=&partialRes;
            //send into the pipeline
            ff_send_out(task);
        }
    return EOS;
    }
};
struct BufferNode: ff::ff_node_t<OrderedArray,task_t>
{
    public:
        task_t* svc(OrderedArray* in){
            if(bufferedValue==std::nullopt){
                bufferedValue=in;
                return GO_ON;
            }

            task_t res(*in,*bufferedValue.value());
            bufferedValue=std::nullopt;
            return &res;

        }
    private:
        std::optional<OrderedArray*> bufferedValue=std::nullopt;
};

struct Worker: ff::ff_node_t<task_t,OrderedArray>
{
    OrderedArray* svc(task_t* in){
        auto [arr1,arr2]=*in;
        OrderedArray res(arr1.size()+arr2.size());
        int i = 0, j = 0, k = 0;
        while (i < arr1.size() && j < arr2.size()) {
            if (arr1[i].key <= arr2[j].key)
                res[k++] = arr1[i++];
            else
                res[k++] = arr2[j++];
        }
        return &res;
    }

};

struct Collector: ff::ff_node_t<OrderedArray,OrderedArray>{
    OrderedArray* svc(OrderedArray* in){
        return in;
    }

};
//parallelized and distributed version using FastFlow+MPI
