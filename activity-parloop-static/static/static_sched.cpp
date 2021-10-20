#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>
#include <thread>
#include <vector>
#include "../sequential/seq_loop.hpp"


//#ifdef __cplusplus
//extern "C" {
//#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);


//using std::chrono::system_clock;

int func, points, intensity;
float lower, upper;
float x;
int i;
float itgr_output, result;

double integrateNum(int func, float lower, float upper, int points, int intensity) {
    if (func == 1) {
        for (i = 0; i <= (points - 1); i++) {
            x = ((lower + (i + .5)) * ((upper - lower) / points));
            itgr_output += f1(x, intensity);
        }
    } else if (func == 2) {
        for (int i = 0; i <= (points - 1); i++) {
            x = ((lower + (i + .5)) * ((upper - lower) / points));
            itgr_output += f2(x, intensity);
        }
    } else if (func == 3) {
        for (int i = 0; i <= (points - 1); i++) {
            x = ((lower + (i + .5)) * ((upper - lower) / points));
            itgr_output += f3(x, intensity);
        }
    } else if (func == 4) {
        for (int i = 0; i <= (points - 1); i++) {
            x = ((lower + (i + .5)) * ((upper - lower) / points));
            itgr_output += f4(x, intensity);
        }
    }

    result = ((upper - lower) / points) * itgr_output;

    return result;
}

int main (int argc, char* argv[]) {

  if (argc < 7) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads>"<<std::endl;
    return -1;
  }

  //scan values from argv[] command line array
  sscanf(argv[1], "%d", &func);
  sscanf(argv[2], "%f", &lower);
  sscanf(argv[3], "%f", &upper);
  sscanf(argv[4], "%d", &points);
  sscanf(argv[5], "%d", &intensity);

  SeqLoop sl;

  auto start = std::chrono::steady_clock::now();

  //integrateNum(func, lower, upper, points, intensity);
  sl.parfor1(0, 5, 1, integrateNum(func, lower, upper, points, intensity));


  auto stop = std::chrono::steady_clock::now();
  std::chrono::duration<double> time_elapsed = stop - start;

  std::cerr << time_elapsed.count()<< "\n";
  std::cout << result << std::endl;

  return 0;
}
