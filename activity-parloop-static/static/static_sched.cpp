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
#include "seq_loop_static.hpp"


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
    int func, points, intensity;
    double lower, upper;
    int nbthreads;
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

    sl.parfor<std::vector<double>>(0, nbthreads, 1, points,
            [&](std::vector<double> & tls) -> void{
                for(int i=0; i < nbthreads; i++) {
                    tls[i] = 0.0;
                }
            },
            [&](int low, int up, std::vector<double> & tls) -> void {

                for (int i = low; i <= up; i++){

                    switch (func) {
                        case 1:
                            tls[i] += f1(lower + (i + 0.5) * ((upper - lower) / points), intensity);
                            break;
                        case 2:
                            tls[i] += f2(lower + (i + 0.5) * ((upper - lower) / points), intensity);
                            break;
                        case 3:
                            tls[i] += f3(lower + (i + 0.5) * ((upper - lower) / points), intensity);
                            break;
                        case 4:
                            tls[i] += f4(lower + (i + 0.5) * ((upper - lower) / points), intensity);
                            break;

                    }
                }

            },
            [&](std::vector<double> tls) -> void{
                for(auto d : tls)
                    sum += d;
            });
  double result = ((upper-lower)/points) * sum;

  auto stop = std::chrono::steady_clock::now();
  std::chrono::duration<double> time_elapsed = stop - start;

  std::cerr << time_elapsed.count()<< "\n";
  std::cout << result << std::endl;

  return 0;
}
