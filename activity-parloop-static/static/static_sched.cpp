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

//using std::chrono::system_clock;

int func, points, intensity, low, up;
float lower, upper;
float x;
int i, nbthreads;
float itgr_output, result;
int sum;
SeqLoop sl;



void integrateNum(int &func, float &lower, float &upper, int &points, int &intensity, std::vector<float> &tls) {
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
    tls[i] = result;
    //return result;
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
  sscanf(argv[6], "%d", &nbthreads);


  int numItr = upper - lower;
  int itrSection, itrRemain;

  itrSection = numItr/nbthreads;
  itrRemain = numItr%nbthreads;

    std::vector<std::thread> parThreads;

  auto start = std::chrono::steady_clock::now();

////Sequential implementation of parfor
//  sl.parfor(0, 1, 1,
//             [&](int i) -> void {
//      integrateNum(func, lower, upper, points, intensity);
//  });

  
    sl.parfor<std::vector<float>>(0, nbthreads, 1,
                   [&](std::vector<float> & tls) -> void{
                    tls[i] = 0.0;
                   },
                   [&](int i, std::vector<float>& tls) -> void {

                       low = i;
                       up = i + (itrSection - 1);

                       if (up > upper) {
                           up = upper;
                       }
                       if ((upper - up) == itrRemain) {
                           up += itrRemain;
                       }
                       std::thread th(&integrateNum, std::ref(func), std::ref(low), std::ref(up), std::ref(points), std::ref(intensity), tls);

                       parThreads.push_back(std::move(th));
                       },
                   [&](std::vector<float>& tls) -> void{

                       for (auto &t: parThreads) {
                           if (t.joinable()) {
                               t.join();
                           }
                           else {
                               std::cout << "t is not joinable" << std::endl;
                           }
                       }

                       for(int k =0; k < nbthreads; k++)
                           sum += tls[k];
                }
    );

//    for (int i = 0, i < numItr; i =(i+itrSection)){
//        for (int j = i; j < i+itrSection; j++ ){
//
//        }
//    }


//    for (int i =0; i < nbthreads; i++){
//        threads[i] = std::thread(threadedParloop, i, lower, upper, points, intensity, std::ref(sum));
//    }

  auto stop = std::chrono::steady_clock::now();
  std::chrono::duration<double> time_elapsed = stop - start;

  std::cerr << time_elapsed.count()<< "\n";
  std::cout << sum << std::endl;

  return 0;
}
