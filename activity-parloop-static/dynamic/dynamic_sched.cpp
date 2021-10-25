#include <iostream>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
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

  if (argc < 8) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <granularity>"<<std::endl;
    return -1;
  }
    int func, points, intensity, chunk;
    double lower, upper;
    int nbthreads, granularity;
    double sum;
    SeqLoop sl;

    //scan values from argv[] command line array
    sscanf(argv[1], "%d", &func);
    sscanf(argv[2], "%lf", &lower);
    sscanf(argv[3], "%lf", &upper);
    sscanf(argv[4], "%d", &points);
    sscanf(argv[5], "%d", &intensity);
    sscanf(argv[6], "%d", &nbthreads);
    sscanf(argv[7], "%d", &granularity);

    auto start = std::chrono::steady_clock::now();

    chunk = (upper-lower)/granularity;

    sl.parfor<double>(0, nbthreads, 1, points, granularity,
                      [&](double & tls) -> void{
                          for(int i=0; i < nbthreads; i++) {
                              tls = 0;

                          }
                      },
                      [&](int low, int up, double & tls) -> void {

                          for (int i = low; i <= up; i++){

                              switch (func) {
                                  case 1:
                                      tls += f1(lower + (i + 0.5) * ((upper - lower) / points), intensity);
                                      break;
                                  case 2:
                                      tls += f2(lower + (i + 0.5) * ((upper - lower) / points), intensity);
                                      break;
                                  case 3:
                                      tls += f3(lower + (i + 0.5) * ((upper - lower) / points), intensity);
                                      break;
                                  case 4:
                                      tls += f4(lower + (i + 0.5) * ((upper - lower) / points), intensity);
                                      break;

                              }
                          }

                      },
                      [&](double tls) -> void{

                          sum += tls;
                      });

    double result = ((upper-lower)/points) * sum;
    auto stop = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_elapsed = stop - start;

    std::cerr << time_elapsed.count()<< "\n";
    std::cout << result << std::endl;

    return 0;
}