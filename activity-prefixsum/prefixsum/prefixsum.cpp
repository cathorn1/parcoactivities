#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include "omploop.hpp"

#ifdef __cplusplus
extern "C" {
#endif
  void generatePrefixSumData (int* arr, size_t n);
  void checkPrefixSumResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  
  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);
  int * arr = new int [n];
  OmpLoop omp;
  omp.setNbThread(nbthreads);


  generatePrefixSumData (arr, n);

    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    //sequential
//    prefix[0] = 0;
//
//    for (int i=0; i<n; ++i) {
//        prefix[i+1] = prefix[i] + arr[i];
//    }

    int* prefix = new int [n+1];
    int *suma; // partial results
    suma = malloc((sizeof *suma) * (nbthreads+1)), suma[0]=0;

    int sum = 0;
    omp.parfor < std::vector < int >> (0, n, 1,
            [&](std::vector<int> &C) -> void {
//                for(int i = 0; i < n; i++){
//                    std::cout << "p1\n";
//                    C.push_back(arr[i]);
//                }

                int ithread = omp_get_thread_num();
                int nthreads = omp_get_num_threads();
            },
            [&](int i, std::vector<int> &C) -> void {
                int ithread = omp_get_thread_num();
                int nthreads = omp_get_num_threads();

                sum += prefix[i] + arr[i];
                suma[ithread + 1] = sum;
//                    std::cout << "middle test\n";
//                    for (int i = 0; i < n; i++) {
//                        std::cout << arr[i] << " ";
//                    }
//                    std::cout << "\n";
                #pragma omp barrier

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
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;

    std::cerr<<elapsed_seconds.count()<<std::endl;
  
  checkPrefixSumResult(prefix, n);

  delete[] arr;

  return 0;
}
