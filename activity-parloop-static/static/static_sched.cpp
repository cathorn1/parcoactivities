#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>
#include <math.h>
#include <thread>
#include <vector>
#include <array>
#include <functional>
#include "../sequential/seq_loop.hpp"


#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

int main (int argc, char* argv[]) {

  if (argc < 7) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads>"<<std::endl;
    return -1;
  }
    int func, points, intensity, low, up;
    double lower, upper;
    double x;
    int i, nbthreads;
    double itgr_output, result;
    double sum;
    SeqLoop sl;

  //scan values from argv[] command line array
  sscanf(argv[1], "%d", &func);
  sscanf(argv[2], "%lf", &lower);
  sscanf(argv[3], "%lf", &upper);
  sscanf(argv[4], "%d", &points);
  sscanf(argv[5], "%d", &intensity);
  sscanf(argv[6], "%d", &nbthreads);

  auto start = std::chrono::steady_clock::now();

////Sequential implementation of parfor
//  sl.parfor(0, 1, 1,
//             [&](int i) -> void {
//      integrateNum(func, lower, upper, points, intensity);
//  });

    sl.parfor<double>(0, nbthreads, 1, points,
                   [&](double & tls) -> void{
                        for(int i=0; i < nbthreads; i++) {
                            tls = 0;
                        }
                   },
                   [&](int low, int up, double & tls) -> void {

                      for (int i = low; i <= up; i++){
                          switch (func) {
                              case 1:
                                  tls += f1(lower + (i + .5) * ((upper - lower) / points), intensity);
                                  break;
                              case 2:
                                  tls += f2(lower + (i + .5) * ((upper - lower) / points), intensity);
                                  break;
                              case 3:
                                  tls += f3(lower + (i + .5) * ((upper - lower) / points), intensity);
                                  break;
                              case 4:
                                  tls += f4(lower + (i + .5) * ((upper - lower) / points), intensity);
                                  break;
                          }
                          }

                       },
                   [&](double tls) -> void{

                           sum += tls;

                }
    );


  auto stop = std::chrono::steady_clock::now();
  std::chrono::duration<double> time_elapsed = stop - start;

  std::cerr << time_elapsed.count()<< "\n";
  std::cout << sum << std::endl;

  return 0;
}
