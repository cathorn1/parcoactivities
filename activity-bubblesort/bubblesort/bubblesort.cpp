#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <vector>
#include "omploop.hpp"

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (const int* arr, size_t n);

#ifdef __cplusplus
}
#endif

void swap(std::vector<int> arr, int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

int main (int argc, char* argv[]) {
  if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);
  OmpLoop omp;
  omp.setNbThread(nbthreads);

  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  //insert sorting code here.


    omp.parfor<std::vector<int>>(0, n, 1,
                   [&](std::vector<int> &C) -> void {
                        for(int i = 0; i < n; i++){
                            C.push_back(arr[i]);
                        }
                   },
                   [&](int a, int b, std::vector<int> &C) -> void {

                       swap(std::ref(C), a, b);

                   },
                   [&](std::vector<int> &C) -> void {
                       for(int i = 0; i < n; i++){
                           arr[i] = C[i];
                       }
                   });



  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end-start;

  checkMergeSortResult (arr, n);
  for (int i =0; i < n; i++) {
      std::cout << arr[i] << " ";
  }
  std::cout << *arr << std::endl;
  std::cerr<<elpased_seconds.count()<<std::endl;

  delete[] arr;

  return 0;
}

//                      for(int k = 0; k < (n-2); k++) {
//                           if (k % 2 == 0) {
//                               for (int i = 0; i <= (n / 2) - 1; i++) {
//                                   if (arr[2 * i] > arr[((2*i)+1)])
//                                       swap(std::ref(arr), (2*i), ((2*i)+1));
//                               }
//                           } else {
//                               for (int i = 0; i <= (n / 2) - 2; i++) {
//                                   if (arr[(2 * i) + 1] > arr[(2 * i) + 2])
//                                       swap(std::ref(arr), ((2 * i)+1), ((2 * i)+2));
//
//                               }
//                           }
//                       }