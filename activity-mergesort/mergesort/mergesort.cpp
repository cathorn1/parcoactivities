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
#include <cstring>
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

//void merge(int *arr, int l, int m, int r) {
//    //std::lock_guard<std::mutex> lck (mut);
//    //mut.lock();
//
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
//
//    //mut.unlock();
//
//}



//void merge(int * arr, int l, int mid, int r) {
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
//    while (i<n && j<=r) {
//        if (temp[i] <= arr[j] ) {
//            arr[k++] = temp[i++];
//        } else {
//            arr[k++] = arr[j++];
//        }
//    }
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
//
//void mergeSort(int * arr, int begin, int end) {
//
//
//    for (int i = begin; i <= end; i++) {
//
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
//                merge(std::ref(arr), left_start, mid+1, right_end);
//            }
//        }
//    }
//}


void mergeSortAux(int *X, int n, int *tmp) {
    int i = 0;
    int j = n/2;
    int ti = 0;

    while (i<n/2 && j<n) {
        if (X[i] < X[j]) {
            tmp[ti] = X[i];
            ti++; i++;
        } else {
            tmp[ti] = X[j];
            ti++; j++;
        }
    }
    while (i<n/2) { /* finish up lower half */
        tmp[ti] = X[i];
        ti++; i++;
    }
    while (j<n) { /* finish up upper half */
        tmp[ti] = X[j];
        ti++; j++;
    }
    memcpy(X, tmp, n*sizeof(int));
}

void mergeSort(int *X, int n, int *tmp)
{
    if (n < 2) return;

#pragma omp task shared(X)
    mergeSort(X, n/2, tmp);

#pragma omp task shared(X)
    mergeSort(X+(n/2), n-(n/2), tmp + n/2);

#pragma omp taskwait
    mergeSortAux(X, n, tmp);
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

   // for (int i =0; i < n; i++) {
//        std::cout << arr[i] << " ";
       // temp[i] = arr[i];
        //std::cout << temp[i] << " ";
//    }
   // std::cout << "\n";


    #if DEBUG
        for (int i=0; i<n; ++i)
            std::cout<<arr[i]<<" ";
    std::cout<<std::endl;
    #endif
    int end_p =0;
    int begin_p =0;
    // begin timing
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    omp.parfor < std::vector < int >> (0, 1, 1,
                    [&](std::vector<int> &C) -> void {
    //                    for(int i = 0; i < n; i++){
    //                        //std::cout << "p1\n";
    //                        C.push_back(arr[i]);
    //                }
                    },
                    [&](int i, std::vector<int> &C) -> void {

                        #pragma omp parallel
                        {
                        #pragma omp single
                            mergeSort(std::ref(arr),n, std::ref(temp));
                        }


//                        int p = omp_get_thread_num();
//                        begin_p = p*(n/nbthreads);
//                        end_p = (p+1) * (n/nbthreads);
//
//                        if (p == nbthreads-1){
//                            end_p += n%nbthreads;
//                        }


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
    //                            merge(std::ref(arr), left_start, mid+1, right_end);
    //                        }
    //                    }

    //                    std::cout << "middle test\n";
    //                    for (int i = 0; i < n; i++) {
    //                        std::cout << arr[i] << " ";
    //                    }
    //                    std::cout << "\n";

                    },
                    [&](std::vector<int> &C) -> void {

                       // mergeSort(std::ref(arr), begin_p, end_p);
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


//    std::cout << "arr test\n";
//    for (int i =0; i < n; i++) {
//        std::cout << arr[i] << " ";
//    }
//    std::cout << "\n";
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



////// lATEST


//        omp.parfor < std::vector < int >> (0, nbthreads, 1,
//                [&](std::vector<int> &C) -> void {
////                for(int i = 0; i < n; i++){
////                    std::cout << "p1\n";
////                    C.push_back(arr[i]);
////                }
//                },
//                [&](int x, std::vector<int> &C) -> void {
//
//                    int p = omp_get_thread_num();
//                    int begin = p*(n/nbthreads);
//                    int end = (p+1) * (n/nbthreads);
//
//                    if (p == nbthreads-1){
//                        end += n%nbthreads;
//                    }
//
//
//                    for (int i = begin; i <= end; i++) {
//
//                        int curr_size;
//                        int left_start;
//                        for (curr_size = 1; curr_size <= i - 1; curr_size = 2 * curr_size) {
//
//                            for (left_start = 0; left_start < i - 1; left_start += 2 * curr_size) {
//
//                                int mid = std::min(left_start + curr_size - 1, i - 1);
//
//                                int right_end = std::min(left_start + 2 * curr_size - 1, i - 1);
//
//                                //std::lock_guard <std::mutex> lck(mut);
//                                merge(std::ref(arr), left_start, mid, right_end);
//
//                            }
//                        }
//                    }
//
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
//
//                });