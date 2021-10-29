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

double f1(float x, int intensity);
double f2(float x, int intensity);
double f3(float x, int intensity);
double f4(float x, int intensity);

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

int main (int argc, char* argv[]) {

    if (argc < 7) {
        std::cerr << "usage: " << argv[0] << " <functionid> <a> <b> <n> <intensity> <nbthreads>" << std::endl;
        return -1;
    }

    int func = atoi(argv[1]);
    double lower = atof(argv[2]);
    double upper = atof(argv[3]);
    int points = atoi(argv[4]);
    int intensity = atoi(argv[5]);
    int nbthreads = atoi(argv[6]);
    double sum;
    SeqLoop sl;

    auto start = std::chrono::steady_clock::now();


    sl.parfor<std::vector<double>>(0, nbthreads, 1, points,
            [&](std::vector<double> &tls) -> void{
                //printf("%s \n", "first func C");
                for(int i=0; i < nbthreads; i++) {
                //   printf("%s \n", "seg fault D");
                    tls.push_back(0.0);
               //    printf("%s \n", "seg fault e");
                }
            },
            [&](int low, int up, int count, std::vector<double> & tls) -> void {
             //  printf("%s \n", "middle func F");
                //for(int i=low; i < up; i++) {
                    //printf("%s %d\n", "seg fault G");

                    tls[count] += integrateNum(func, points, up, low, intensity);
                    printf("tls[%d] %f ", count, tls[count]);
                    //printf("%s %d\n", "seg fault H");
                //}


            },
            [&](std::vector<double> &tls) -> void{
              // printf("%s \n", "last func");
                for(auto d : tls) {
                    //printf("%s \n", "seg fault J");
                    sum += d;

                    //printf("%s \n", "seg fault K");
                }

            });

    auto stop = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_elapsed = stop - start;

    std::cerr << time_elapsed.count() << "\n";
    std::cout << sum << std::endl;

    return 0;
}

//    sl.parfor<std::vector<double>>(0, nbthreads, 1, points,
//            [&](std::vector<double> &tls)  -> void{
//
//                for(int i=0; i < nbthreads; i++) {
//                    tls.push_back(0.0);
//                }
//            },
//            [&](int low, int up, std::vector<double> & tls)  -> void {
//
//                for (int i = 0; i <= up; i++){
//                    printf("%s %d\n", "seg fault G", i);
//                    switch (func) {
//                        case 1:
//                            printf("%s %d\n", "case-1", i);
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
//            [&](std::vector<double> &tls)  -> void{
//                for(double d : tls)
//                    sum += d;
//            });
//
//    double result = ((upper-lower)/points) * sum;



//    sl.parfor<std::vector<double>>(0, nbthreads, 1, points,
//            [&](std::vector<double> &tls) -> void{
////                printf("%s \n", "seg fault C");
//                for(int i=0; i < nbthreads; i++) {
////                    printf("%s \n", "seg fault D");
//                    tls.push_back(0.0);
////                    printf("%s \n", "seg fault e");
//                }
//            },
//            [&](int low, int up, std::vector<double> & tls) -> void {
////                printf("%s \n", "seg fault F");
//                for(int i=low; i < up; i++) {
////                    printf("%s %d\n", "seg fault G", i);
//                    tls.push_back(integrateNum(func, points, upper, lower, intensity));
////                    printf("%s %d\n", "seg fault H", i);
//                }
//
//            },
//            [&](std::vector<double> &tls) -> void{
////                printf("%s \n", "seg fault I");
//                for(auto d : tls) {
//                    //printf("%s \n", "seg fault J");
//                    sum += d;
//                    //printf("%s \n", "seg fault K");
//                }
//            });





//    sl.parfor<std::vector<double>>(0, nbthreads, 1, points,
//            [&](std::vector<double> &tls) mutable -> void{
//
//            for(int i=0; i < nbthreads; i++) {
//                    tls[i] = 0.0;
//                }
//            },
//            [&](int low, int up, std::vector<double> & tls) mutable -> void {
//
//                for (int i = low; i <= up; i++){
//
//                    switch (func) {
//                        case 1:
//                            tls[0] += f1(lower + (i + 0.5) * ((upper - lower) / points), intensity);
//                            break;
//                        case 2:
//                            tls[1] += f2(lower + (i + 0.5) * ((upper - lower) / points), intensity);
//                            break;
//                        case 3:
//                            tls[2] += f3(lower + (i + 0.5) * ((upper - lower) / points), intensity);
//                            break;
//                        case 4:
//                            tls[3] += f4(lower + (i + 0.5) * ((upper - lower) / points), intensity);
//                            break;
//
//                    }
//                }
//
//            },
//            [&](std::vector<double> &tls) mutable -> void{
//                for(auto d : tls)
//                    sum += d;
//            });
//  double result = ((upper-lower)/points) * sum;