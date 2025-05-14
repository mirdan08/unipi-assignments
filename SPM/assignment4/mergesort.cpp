#include <cstddef>
#include <vector>

#include "mergesort.hpp"



//sequenmtial merge sort implementation
void sequentialMerge(std::vector<int>& arr, int left, int mid, int right) {
    // Create temporary arrays
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1), R(n2);

    // Copy data to temp arrays
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

//parallelized and distributed version using FastFlow+MPI
