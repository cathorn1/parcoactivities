#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <chrono>
#include <mutex>
#include <omp.h>
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

//void merge(int arr[], int l, int m, int r) {
//    //std::lock_guard<std::mutex> lck (mut);
//    int i, j, k;
//    int n1 = m - l + 1;
//    int n2 =  r - m;
//
//    int L[n1], R[n2];
//
//    for (i = 0; i < n1; i++)
//        L[i] = arr[l + i];
//    for (j = 0; j < n2; j++)
//        R[j] = arr[m + 1+ j];
//
//    i = 0;
//    j = 0;
//    k = l;
//    while (i < n1 && j < n2)
//    {
//        if (L[i] <= R[j])
//        {
//            arr[k] = L[i];
//            i++;
//        }
//        else
//        {
//            arr[k] = R[j];
//            j++;
//        }
//        k++;
//    }
//
//    while (i < n1)
//    {
//        arr[k] = L[i];
//        i++;
//        k++;
//    }
//
//    while (j < n2)
//    {
//        arr[k] = R[j];
//        j++;
//        k++;
//    }
//}
//
//void mergeSort(int arr[], int begin, int end) {
//
//
//    for (int i = begin; i <= end; i++) {
//        int curr_size;
//        int left_start;
//        for (curr_size = 1; curr_size <= i - 1; curr_size = 2 * curr_size) {
//
//            for (left_start = 0; left_start < i - 1; left_start += 2 * curr_size) {
//
//                int mid = std::min(left_start + curr_size - 1, i - 1);
//
//                int right_end = std::min(left_start + 2 * curr_size - 1, i - 1);
//
//                //std::lock_guard <std::mutex> lck(mut);
//                merge(std::ref(arr), left_start, mid, right_end);
//            }
//        }
//    }
//}
void merge(int * arr, int l, int mid, int r) {

#if DEBUG
    std::cout<<l<<" "<<mid<<" "<<r<<std::endl;
#endif

    // short circuits
    if (l == r) return;
    if (r-l == 1) {
        if (arr[l] > arr[r]) {
            int temp = arr[l];
            arr[l] = arr[r];
            arr[r] = temp;
        }
        return;
    }

    int i, j, k;
    int n = mid - l;

    // declare and init temp arrays
    int *temp = new int[n];
    for (i=0; i<n; ++i)
        temp[i] = arr[l+i];

    i = 0;    // temp left half
    j = mid;  // right half
    k = l;    // write to

    // merge
    while (i<n && j<=r) {
        if (temp[i] <= arr[j] ) {
            arr[k++] = temp[i++];
        } else {
            arr[k++] = arr[j++];
        }
    }

    // exhaust temp
    while (i<n) {
        arr[k++] = temp[i++];
    }

    // de-allocate structs used
    delete[] temp;

}
//void merge(int arr[], int temp[], int from, int mid, int to)
//{
//    int k = from, i = from, j = mid + 1;
//
//    // loop till no elements are left in the left and right runs
//    while (i <= mid && j <= to)
//    {
//        if (arr[i] < arr[j]) {
//            temp[k++] = arr[i++];
//        }
//        else {
//            temp[k++] = arr[j++];
//        }
//    }
//
//    // copy remaining elements
//    while (i < (to-from) && i <= mid) {
//        temp[k++] = arr[i++];
//    }
//
//    /* no need to copy the second half (since the remaining items
//       are already in their correct position in the temporary array) */
//
//    // copy back to the original array to reflect sorted order
//    for (int i = from; i <= to; i++) {
//        arr[i] = temp[i];
//    }
//}

// Iteratively sort subarray `A[low…high]` using a temporary array
void mergesort(int arr[], int low, int high)
{
    // divide the array into blocks of size `m`
    // m = [1, 2, 4, 8, 16…]
    for (int m = 1; m <= high - low; m = 2*m)
    {
        // for m = 1, i = 0, 2, 4, 6, 8…
        // for m = 2, i = 0, 4, 8…
        // for m = 4, i = 0, 8…
        // …
        for (int i = low; i < high; i += 2*m)
        {
            int from = i;
            int mid = i + m - 1;
            int to = std::min(i + 2*m - 1, high);

            merge(std::ref(arr), from, mid, to);
        }
    }
}

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
  int* temp = new int[n];
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


        omp.parfor < std::vector < int >> (0, nbthreads, 1,
                [&](std::vector<int> &C) -> void {
//                for(int i = 0; i < n; i++){
//                    std::cout << "p1\n";
//                    C.push_back(arr[i]);
//                }
                },
                [&](int i, std::vector<int> &C) -> void {

                    int p = omp_get_thread_num();
                    //std::cout << "p val: " << p << "\n";

                    int begin = p*(n/nbthreads);
                    int end = (p+1) * (n/nbthreads);

                    if (p == nbthreads-1){
                        end += n%nbthreads;
                    }
//                    std::lock_guard<std::mutex> lk (mut);
//                    mergeSort(std::ref(arr), begin, end);

                    for (int i = begin; i <= end; i++) {
                        mergesort(std::ref(arr), begin, end);

                    }

            //                    int curr_size;
//                    int left_start;
//                    for (curr_size=1; curr_size<=i-1; curr_size = 2*curr_size) {
//
//                        for (left_start = 0; left_start < i - 1; left_start += 2 * curr_size) {
//
//                            int mid = std::min(left_start + curr_size - 1, i - 1);
//
//                            int right_end = std::min(left_start + 2 * curr_size - 1, i - 1);
//
//                            std::lock_guard<std::mutex> lck (mut);
//                            merge(std::ref(arr), left_start, mid, right_end);
//                        }
//                    }

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

//omp.parfor < std::vector < int >> (0, n+1, 1,
//                [&](std::vector<int> &C) -> void {
////                for(int i = 0; i < n; i++){
////                    std::cout << "p1\n";
////                    C.push_back(arr[i]);
////                }
//                },
//                [&](int i, std::vector<int> &C) -> void {
//
//                    int curr_size;
//                    int left_start;
//                    for (curr_size=1; curr_size<=i-1; curr_size = 2*curr_size) {
//
//                        for (left_start = 0; left_start < i - 1; left_start += 2 * curr_size) {
//
//                            int mid = std::min(left_start + curr_size - 1, i - 1);
//
//                            int right_end = std::min(left_start + 2 * curr_size - 1, i - 1);
//
//                            std::lock_guard<std::mutex> lck (mut);
//                            merge(std::ref(arr), left_start, mid, right_end);
//                        }
//                    }
//
////                    std::cout << "middle test\n";
////                    for (int i = 0; i < n; i++) {
////                        std::cout << arr[i] << " ";
////                    }
////                    std::cout << "\n";
//
//                },
//                [&](std::vector<int> &C) -> void {
////                for(int i = 0; i < n; i++){
////                    //std::cout << "p3\n";
////                    arr[i] = C[i];
////                }
//
////                       std::cout << "test C\n";
////                       for (int i =0; i < n; i++) {
////                           std::cout << arr[i] << " ";
////                       }
////                       std::cout << "\n";
//                });
