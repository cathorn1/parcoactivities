#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <chrono>
#include <mutex>
#include "omploop.hpp"

#ifdef __cplusplus
extern "C" {
#endif
  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif

std::mutex mut;

//void merge(int arr[], int l, int mid, int r) {
//
//#if DEBUG
//    std::cout<<l<<" "<<mid<<" "<<r<<std::endl;
//#endif
//
//    // short circuits
//    if (l == r) return;
//    if (r-l == 1) {
//        if (arr[l] > arr[r]) {
//            int temp = arr[l];
//            arr[l] = arr[r];
//            arr[r] = temp;
//        }
//        return;
//    }
//
//    int i, j, k;
//    int n = mid - l;
//
//    // declare and init temp arrays
//    int *temp = new int[n];
//    for (i=0; i<n; ++i)
//        temp[i] = arr[l+i];
//
//    i = 0;    // temp left half
//    j = mid;  // right half
//    k = l;    // write to
//
//    // merge
//
//    while (i<n && j<=r) {
//        if (temp[i] <= arr[j] ) {
//            arr[k++] = temp[i++];
//        } else {
//            arr[k++] = arr[j++];
//        }
//    }
//
//
//    // exhaust temp
//    while (i<n) {
//        arr[k++] = temp[i++];
//    }
//
//    // de-allocate structs used
//    delete[] temp;
//
//}

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    int L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}
void mergeSort(int arr[], int n)
{

    int curr_size;  // For current size of subarrays to be merged
    // curr_size varies from 1 to n/2
    int left_start; // For picking starting index of left subarray
    // to be merged

    // Merge subarrays in bottom up manner.  First merge subarrays of
    // size 1 to create sorted subarrays of size 2, then merge subarrays
    // of size 2 to create sorted subarrays of size 4, and so on.
    for (curr_size=1; curr_size<=n-1; curr_size = 2*curr_size)
    {
        // Pick starting point of different subarrays of current size
        for (left_start=0; left_start<n-1; left_start += 2*curr_size)
        {
            // Find ending point of left subarray. mid+1 is starting
            // point of right
            int mid = std::min(left_start + curr_size - 1, n-1);

            int right_end = std::min(left_start + 2*curr_size - 1, n-1);

            // Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end]
            //std::lock_guard<std::mutex> lck (mut);
            merge(std::ref(arr), left_start, mid, right_end);
        }
    }
}

//void merge(int arr[], int left, int middle, int right) {
//    int i = 0;
//    int j = 0;
//    int k = 0;
//    int left_length = middle - left + 1;
//    int right_length = right - middle;
//    int left_array[left_length];
//    int right_array[right_length];
//
//    /* copy values to left array */
//    for (int i = 0; i < left_length; i ++) {
//        left_array[i] = arr[left + i];
//    }
//
//    /* copy values to right array */
//    for (int j = 0; j < right_length; j ++) {
//        right_array[j] = arr[middle + 1 + j];
//    }
//
//    i = 0;
//    j = 0;
//    /** chose from right and left arrays and copy */
//    while (i < left_length && j < right_length) {
//        if (left_array[i] <= right_array[j]) {
//            arr[left + k] = left_array[i];
//            i ++;
//        } else {
//            arr[left + k] = right_array[j];
//            j ++;
//        }
//        k ++;
//    }
//
//    /* copy the remaining values to the array */
//    while (i < left_length) {
//        arr[left + k] = left_array[i];
//        k ++;
//        i ++;
//    }
//    while (j < right_length) {
//        arr[left + k] = right_array[j];
//        k ++;
//        j ++;
//    }
//}
//
//void merge_sections_of_array(int arr[], int n, int nbthread, int chunk, int aggregation) {
//    for(int i = 0; i < nbthread; i = i + 2) {
//        int left = i * (chunk * aggregation);
//        int right = ((i + 2) * chunk * aggregation) - 1;
//        int middle = left + (chunk * aggregation) - 1;
//        if (right >= n) {
//            right = n - 1;
//        }
//        //merge(arr, left, middle, right);
//        merge(std::ref(arr), left, middle, right);
//    }
//    if (nbthread / 2 >= 1) {
//        merge_sections_of_array(std::ref(arr), n, nbthread / 2, chunk, aggregation * 2);
//    }
//}

int main (int argc, char* argv[]) {
  
  if (argc < 3) { std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);
  OmpLoop omp;
  omp.setNbThread(nbthreads);

  int chunk = n/nbthreads;
  int chunkRemain = n%nbthreads;

  // get arr data
  int* arr = new int [n];
  generateMergeSortData (arr, n);

    for (int i =0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";


    #if DEBUG
        for (int i=0; i<n; ++i)
            std::cout<<arr[i]<<" ";
    std::cout<<std::endl;
    #endif

    // begin timing
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();


    //for(int i =0; i < n; i++) {

    int curr_size = 1;
    //int left_start = 0;

        omp.parfor < std::vector < int >> (curr_size, n - 1, 2*curr_size,
                [&](std::vector<int> &C) -> void {
//                for(int i = 0; i < n; i++){
//                    std::cout << "p1\n";
//                    C.push_back(arr[i]);
//                }
                },
                [&](int i, std::vector<int> &C) -> void {


                    //int curr_size;  // For current size of subarrays to be merged
                    // curr_size varies from 1 to n/2
                    int left_start; // For picking starting index of left subarray
                    // to be merged

                    // Merge subarrays in bottom up manner.  First merge subarrays of
                    // size 1 to create sorted subarrays of size 2, then merge subarrays
                    // of size 2 to create sorted subarrays of size 4, and so on.

                    //for (curr_size = 1; curr_size <= i - 1; curr_size = 2 * curr_size) {
                        // Pick starting point of different subarrays of current size
                        for (left_start = 0; left_start < n - 1; left_start += 2 * curr_size) {
                            // Find ending point of left subarray. mid+1 is starting
                            // point of right
                            int mid = std::min(left_start + curr_size - 1, n - 1);

                            int right_end = std::min(left_start + 2 * curr_size - 1, n - 1);

                            // Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end]
                            //std::lock_guard <std::mutex> lck(mut);
                            //mut.lock();
                            merge(std::ref(arr), left_start, mid, right_end);
                            //mut.unlock();
                        }

                    //}
//

                    std::cout << "middle test\n";
                    for (int i = 0; i < n; i++) {
                        std::cout << arr[i] << " ";
                    }
                    std::cout << "\n";

                },
                [&](std::vector<int> &C) -> void {
//                for(int i = 0; i < n; i++){
//                    //std::cout << "p3\n";
//                    arr[i] = C[i];
//                }

//                       std::cout << "test C\n";
//                       for (int i =0; i < n; i++) {
//                           std::cout << arr[i] << " ";
//                       }
//                       std::cout << "\n";
                });
   // } //close outer for

    // end timing
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> elpased_seconds = end-start;

    // display time to cerr
    std::cerr<<elpased_seconds.count()<<std::endl;
    checkMergeSortResult (arr, n);

  #if DEBUG
        for (int i=0; i<n; ++i)
        std::cout<<arr[i]<<" ";
      std::cout<<std::endl;
  #endif

  delete[] arr;

  return 0;
}



//int curr_size;  // For current size of subarrays to be merged
//                // curr_size varies from 1 to n/2
//                int left_start; // For picking starting index of left subarray
//                // to be merged
//
//                // Merge subarrays in bottom up manner.  First merge subarrays of
//                // size 1 to create sorted subarrays of size 2, then merge subarrays
//                // of size 2 to create sorted subarrays of size 4, and so on.
//                for (curr_size=1; curr_size<=n-1; curr_size = 2*curr_size)
//                {
//                    // Pick starting point of different subarrays of current size
//                    for (left_start=0; left_start<n-1; left_start += 2*curr_size)
//                    {
//                        // Find ending point of left subarray. mid+1 is starting
//                        // point of right
//                        int mid = std::min(left_start + curr_size - 1, n-1);
//
//                        int right_end = std::min(left_start + 2*curr_size - 1, n-1);
//
//                        // Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end]
//                        //std::lock_guard<std::mutex> lck (mut);
//                        merge(std::ref(arr), left_start, (mid+1), right_end);
//                    }
//                }



