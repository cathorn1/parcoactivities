#include <iostream>
#include <stdio.h>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <vector>
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

double integrateNum (int func, int points, double upper, double lower, int intensity) {

    double itgr_output = 0.0;
    double x = 0.0;

    if (func == 1) {
        for (int i = 0; i <= (points - 1); i++) {
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

    double result = ((upper - lower) / points) * itgr_output;
    return result;
}

int func, points, intensity, nbthreads, granularity;
double lower, upper;

int main (int argc, char* argv[]) {

  if (argc < 8) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <granularity>"<<std::endl;
    return -1;
  }

    std::vector<double> tlsVec;

    printf("%s \n", "seg fault A");

    func = atoi(argv[1]);
    lower = atof(argv[2]);
    upper = atof(argv[3]);
    points = atoi(argv[4]);
    intensity = atoi(argv[5]);
    nbthreads = atoi(argv[6]);
    granularity = atoi(argv[7]);

    double sum;
    SeqLoop sl;

    auto start = std::chrono::steady_clock::now();

    double chunk = (upper-lower)/granularity;

    printf("%s \n", "seg fault B");

    sl.parfor<std::vector<double>>(0, nbthreads, 1, points, granularity,
            [&](std::vector<double> &tls) mutable -> void{
                printf("%s \n", "seg fault C");
                for(int i=0; i < nbthreads; i++) {
                    printf("%s \n", "seg fault D");
                    tls.push_back(0.0);
                    printf("%s \n", "seg fault e");
                }
            },
            [&](int low, int up, std::vector<double> & tls) mutable -> void {
                printf("%s \n", "seg fault F");
                for(int i=low; i < up; i++) {
                    printf("%s \n", "seg fault G");
                    tls.push_back(integrateNum(func, points, up, low, intensity));
                    printf("%s \n", "seg fault H");
                }

            },
            [&](std::vector<double> &tls) mutable -> void{
                printf("%s \n", "seg fault I");
                for(auto d : tls) {
                    printf("%s \n", "seg fault J");
                    sum += d;
                    printf("%s \n", "seg fault K");
                }
            });

    auto stop = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_elapsed = stop - start;

    std::cerr << time_elapsed.count()<< "\n";
    std::cout << sum << std::endl;

    return 0;
}




//    sl.parfor<std::vector<double>>(0, nbthreads, 1, points, granularity,
//            [&](std::vector<double> & tls) -> void{
//                for(int i=0; i < nbthreads; i++) {
//                    tls[i] = 0.0;
//                }
//            },
//            [&](int low, int up, std::vector<double> & tls) -> void {
//
//                for (int i = low; i <= up; i++){
//
//                    switch (func) {
//                        case 1:
//                            tls[i] += f1(lower + (i + 0.5) * ((upper - lower) / points), intensity);
//                            break;
//                        case 2:
//                            tls[i] += f2(lower + (i + 0.5) * ((upper - lower) / points), intensity);
//                            break;
//                        case 3:
//                            tls[i] += f3(lower + (i + 0.5) * ((upper - lower) / points), intensity);
//                            break;
//                        case 4:
//                            tls[i] += f4(lower + (i + 0.5) * ((upper - lower) / points), intensity);
//                            break;
//
//                    }
//                }
//
//            },
//            [&](std::vector<double> tls) -> void{
//                for(auto d : tls)
//                    sum += d;
//            });
//
//    double result = ((upper-lower)/points) * sum;



