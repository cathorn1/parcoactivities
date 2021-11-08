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

void merge(int arr[], int temp[], int l, int m, int r) {
    //std::lock_guard<std::mutex> lck (mut);
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = temp[l + i];
    for (j = 0; j < n2; j++)
        R[j] = temp[m + 1+ j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            temp[k] = L[i];
            i++;
        }
        else
        {
            temp[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        temp[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        temp[k] = R[j];
        j++;
        k++;
    }

    for(int k = 0; k < (r-l)+1; k++){
       mut.lock();
       arr[k] = temp[k];
       mut.unlock();
    }
}

void mergeSort(int arr[], int temp[], int begin, int end) {


    for (int i = begin; i <= end; i++) {
        int curr_size;
        int left_start;
        for (curr_size = 1; curr_size <= i - 1; curr_size = 2 * curr_size) {

            for (left_start = 0; left_start < i - 1; left_start += 2 * curr_size) {

                int mid = std::min(left_start + curr_size - 1, i - 1);

                int right_end = std::min(left_start + 2 * curr_size - 1, i - 1);

                //std::lock_guard <std::mutex> lck(mut);
                merge(std::ref(arr), temp, left_start, mid, right_end);
            }
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
//        std::cout << arr[i] << " ";
//        temp[i] = arr[i];
        std::cout << temp[i] << " ";
    }
   // std::cout << "\n";


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

                    mergeSort(std::ref(arr), std::ref(temp), begin, end);


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
