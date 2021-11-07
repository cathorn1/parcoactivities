#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <omp.h>
#include <vector>
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
    int* pre = new int [n+1];
    pre[0] = 0;
    std::cout << "print arr" << std::endl;
    for (int i=0; i<n; ++i) {
        std::cout << arr[i] << " ";
    }
    for (int i=0; i<n; ++i) {
        pre[i+1] = pre[i] + arr[i];
    }
    std::cout << "\nprint sequential pre test" << std::endl;
    for (int i=0; i<n+1; ++i) {
        std::cout << pre[i] << " ";
    }

    int* prefix = new int [n+1];
    int *suma = new int[nbthreads+1]; // partial results

    prefix[0] = 0;
    int s = 0;
    int offset = 0;
    omp.parfor <std::vector<int>> (0, n, 1,
            [&](std::vector<int> &C) -> void {
//                for(int i = 0; i < n; i++){
//                    std::cout << "p1\n";
//                    C.push_back(arr[i]);
//                }

                //int ithread = omp_get_thread_num();
                //int nthreads = omp_get_num_threads();
            },
            [&](int i, std::vector<int> &C) -> void {
                int ithread = omp_get_thread_num();
                int nthreads = omp_get_num_threads();

                s += prefix[i] + arr[i];

                suma[ithread + 1] = s;
//                    std::cout << "middle test\n";
//                    for (int i = 0; i < n; i++) {
//                        std::cout << arr[i] << " ";
//                    }
//                    std::cout << "\n";
                #pragma omp barrier

                for(int i=0; i<(ithread+1); i++)
                    offset += suma[i];

            },
            [&](std::vector<int> &C) -> void {

                for (int i=0; i<n; i++)
                    prefix[i] += offset;

                std::cout << "\nprint prefix hopefully" << std::endl;
                for (int i=0; i<n+1; ++i) {
                    std::cout << prefix[i] << " ";
                }
            });
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;

    std::cerr<<elapsed_seconds.count()<<std::endl;
  
  checkPrefixSumResult(prefix, n);

  delete[] arr;

  return 0;
}
