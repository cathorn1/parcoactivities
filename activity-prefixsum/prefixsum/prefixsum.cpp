#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <omp.h>
#include <vector>
#include <cmath>
#include <math.h>
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
    //int *suma = new int[nbthreads+1]; // partial results

    prefix[0] = 0;
    //int s = 0;
    //int offset = 0;

    //arr[n-1] =0;

    //int chunk = pow(2,nbthreads +1);

    for (int i = 0; i < (log(n)-1); i++) {

        omp.parfor<std::vector<int>>(0, n-1, pow(2, i+1),
                [&](std::vector<int> &C) -> void {

                },
                [&](int k, std::vector<int> &C) -> void {

                      //prefix[k] = arr[k];
//                      if (k >= pow(2, i)){
//                          int t = pow(2, i);
//                          prefix[k] = arr[(k-t)] + arr[k];
//                      }
//                      else{
//                          prefix[k] = arr[k];
//                      }

                    int indA = k + pow(2, i) - 1;
                    int indB = k +  pow(2, (i+1)) - 1;
                    int temp = arr[indA];
                    //arr[indA] = arr[indB];
                    arr[indB] = temp + arr[indB];
                },
                [&](std::vector<int> &C) -> void {

//                    for (int i = 0; i < n; i++)
//                        prefix[i] += offset;

//                    std::cout << "\nprint arr" << std::endl;
//                    for (int i = 0; i < n + 1; ++i) {
//                        std::cout << arr[i] << " ";
//                    }


                });
   }

    for (int i = (log(n)-1); i >= 0 ; i--) {

        omp.parfor<std::vector<int>>(0, n-1, pow(2, i+1),
                [&](std::vector<int> &C) -> void {

                },
                [&](int k, std::vector<int> &C) -> void {


                    int indA = k + pow(2, i) - 1;
                    int indB = k +  pow(2, (i+1)) - 1;
                    int temp = arr[indA];
                    arr[indA] = arr[indB];
                    arr[indB] = temp + arr[indB];
                },
                [&](std::vector<int> &C) -> void {

                });
    }





    std::cout << "\nprint arr" << std::endl;
    for (int i = 0; i < n + 1; ++i) {
        std::cout << arr[i] << " ";
    }

    std::cout<<std::endl;

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;

    std::cerr<<elapsed_seconds.count()<<std::endl;
  
  checkPrefixSumResult(arr, n);

  delete[] arr;

  return 0;
}



//omp.parfor <std::vector<int>> (0, n, 1,
//            [&](std::vector<int> &C) -> void {
////                for(int i = 0; i < n; i++){
////                    std::cout << "p1\n";
////                    C.push_back(arr[i]);
////                }
//
//                //int ithread = omp_get_thread_num();
//                //int nthreads = omp_get_num_threads();
//            },
//            [&](int i, std::vector<int> &C) -> void {
//                int ithread = omp_get_thread_num();
//                int nthreads = omp_get_num_threads();
//
//                s += arr[i];
//                prefix[i] += prefix[i] + arr[i];
//                suma[ithread + 1] = s;
////                    std::cout << "middle test\n";
////                    for (int i = 0; i < n; i++) {
////                        std::cout << arr[i] << " ";
////                    }
////                    std::cout << "\n";
//                #pragma omp barrier
//
//                for(int i=0; i<(ithread+1); i++)
//                    offset += suma[i];
//
//            },
//            [&](std::vector<int> &C) -> void {
//
//                for (int i=0; i<n; i++)
//                    prefix[i] += offset;
//
//                std::cout << "\nprint suma" << std::endl;
//                for (int i=0; i<n+1; ++i) {
//                    std::cout << suma[i] << " ";
//                }
//                std::cout << "\nprint prefix" << std::endl;
//                for (int i=0; i<n+1; ++i) {
//                    std::cout << prefix[i] << " ";
//                }
//            });