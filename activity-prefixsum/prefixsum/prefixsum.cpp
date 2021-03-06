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
#include <mutex>
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
  std::mutex mut;
  omp.setNbThread(nbthreads);


  generatePrefixSumData (arr, n);

    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    //sequential
//   int* pre = new int [n+1];
//    pre[0] = 0;
//    std::cout << "print arr" << std::endl;
//    for (int i=0; i<n; ++i) {
//        std::cout << arr[i] << " ";
//    }
//    for (int i=0; i<n; ++i) {
//        pre[i+1] = pre[i] + arr[i];
//    }
//    std::cout << "\nprint sequential pre test" << std::endl;
//    for (int i=0; i<n+1; ++i) {
//        std::cout << pre[i] << " ";
//    }

    int* prefix = new int [n+1];

    prefix[0] = 0;

    //int chunk = pow(2,nbthreads +1);
   if (n > 1000) {

            omp.parfor < std::vector < int >> (0, 1, 1,
                    [&](std::vector<int> &C) -> void {

                    },
                    [&](int x, std::vector<int> &C) -> void {

                        for (int d = 0; d <= (log10(n) - 1); d++) {

                            for(int i = 0; i < n-1; i+=pow(2, d + 1)) {
                                int indA = i + pow(2, d) - 1;
                                int indB = i + pow(2, (d + 1)) - 1;

                                //mut.lock();
                                //std::lock_guard<std::mutex> lck(mut);

                                int temp = arr[indA];
                                //arr[indA] = arr[indB];
                                arr[indB] = temp + arr[indB];

                                //mut.unlock();
                            }
                        }


                        for (int d = (log10(n) - 1); d >= 0; d--) {

                            for(int i = 0; i < n-1; i+=pow(2, d + 1)) {
                                int indA = i + pow(2, d) - 1;
                                int indB = i + pow(2, (d + 1)) - 1;

                                //mut.lock();
                                //std::lock_guard<std::mutex> lck(mut);

                                int temp = arr[indA];
                                arr[indA] = arr[indB];
                                arr[indB] = temp + arr[indB];

                                //mut.unlock();
                            }
                        }
                    },
                    [&](std::vector<int> &C) -> void {

                    });


        for (int i = 0; i <=n; i++) {
            prefix[i] = arr[i];
        }
        if ((n-1)%2 ==0) {
            std::swap(prefix[n - 1], prefix[n]);
        }
    } else {

       int c = 1;
       int sum = 0;
       while (c < n) {
           sum = arr[c - 1] + arr[c];
           arr[c] = sum;
           c++;
       }

       for (int i = 0; i < n; i++) {
           prefix[i + 1] = arr[i];
       }
   }

//    std::cout << "\nprint prefix" << std::endl;
//    for (int i = 0; i < n + 1; ++i) {
//        std::cout << prefix[i] << " ";
//    }
//    std::cout<<std::endl;
//    std::cout << "\nprint arr" << std::endl;
//    for (int i = 0; i < n + 1; ++i) {
//        std::cout << arr[i] << " ";
//    }
//    std::cout<<std::endl;

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;

    std::cerr<<elapsed_seconds.count()<<std::endl;
  
  checkPrefixSumResult(prefix, n);

  delete[] arr;

  return 0;
}


//if (n > 1000) {
//        for (int i = 0; i <= (log10(n) - 1); i++) {
//
//            omp.parfor < std::vector < int >> (0, n - 1, pow(2, i + 1),
//                    [&](std::vector<int> &C) -> void {
//
//                    },
//                    [&](int k, std::vector<int> &C) -> void {
//
//                        int indA = k + pow(2, i) - 1;
//                        int indB = k + pow(2, (i + 1)) - 1;
//                        int temp = arr[indA];
//                        //arr[indA] = arr[indB];
//                        arr[indB] = temp + arr[indB];
//                    },
//                    [&](std::vector<int> &C) -> void {
//
//                    });
//        }
//
//        for (int i = (log10(n) - 1); i >= 0; i--) {
//
//            omp.parfor < std::vector < int >> (0, n - 1, pow(2, i + 1),
//                    [&](std::vector<int> &C) -> void {
//
//                    },
//                    [&](int k, std::vector<int> &C) -> void {
//
//                        int indA = k + pow(2, i) - 1;
//                        int indB = k + pow(2, (i + 1)) - 1;
//                        int temp = arr[indA];
//                        arr[indA] = arr[indB];
//                        arr[indB] = temp + arr[indB];
//                    },
//                    [&](std::vector<int> &C) -> void {
//
//                    });
//        }
//
//        for (int i = 0; i <=n; i++) {
//            prefix[i] = arr[i];
//        }
//        if ((n-1)%2 ==0) {
//            std::swap(prefix[n - 1], prefix[n]);
//        }
//    } else{
//
//        int c = 1;
//        int sum = 0;
//        while(c < n){
//            sum = arr[c-1] + arr[c];
//            arr[c] = sum;
//            c++;
//        }
//
//        for (int i = 0; i < n; i++){
//            prefix[i+1] = arr[i];
//        }
//      }

