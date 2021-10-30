#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <array>
#include <chrono>
#include <algorithm>
#include "omploop.hpp"

#ifdef __cplusplus
extern "C" {
#endif

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

#ifdef __cplusplus
}
#endif



int main (int argc, char* argv[]) {

    if (argc < 4) {
        std::cerr << "usage: " << argv[0] << " <m> <n> <nbthreads>" << std::endl;
        return -1;
    }

    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int nbthreads = atoi(argv[3]);
    int answer;
    OmpLoop om;

    // get string data
    char *X = new char[m];
    char *Y = new char[n];
    generateLCS(X, m, Y, n);
    //insert LCS code here.

    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    om.parfor < std::vector < std::vector < int>>>(0, nbthreads, 1, m, n, X, Y,
            [&](std::vector <std::vector<int>> &tls) -> void {
                for (int i = 0; i < m; ++i) {
                    std::vector<int> vec(1, 0);
                    tls.push_back(vec);
                    tls[i][0] = 0;
                    printf("%s %d\n", "howdy 1", i);
                }
                for (int j = 0; j < n; ++j) {
                    printf("%s %d\n", "howdy 2", j);
                    tls[0][j] = 0;
                }
            },
            [&](int a, int b, char* U, char* W, std::vector <std::vector<int>> &tls) -> void {
                printf("%s\n", "howdy 3");
                if (U[a - 1] == W[b - 1]) {
                    printf("%s a: %d b: %d\n", "howdy 4", a, b);
                    tls[a][b] = tls[a - 1][b - 1] + 1;
                } else {
                    printf("%s a: %d b: %d\n", "howdy 5", a, b);
                    tls[a][b] = std::max(tls[a - 1][b], tls[a][b - 1]);
                }
                printf("%s %d\n", "from middle ", tls[m][n]);

            },
            [&](std::vector <std::vector<int>> &tls) -> void {
                printf("%s\n", "howdy 6");
                answer = tls[m][n];
                printf("%s %d\n", "from last ", tls[m][n]);
                printf("%s\n", "howdy 77");

//                    for (int i=0; i<=m; ++i) {
//                        delete tls[i];
//                    }
//                    delete tls;
            });



    int result = answer; // length of common subsequence
    printf("%s", "made bottom");
    printf("%s %d\n", "from end", result);

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> elpased_seconds = end-start;

    checkLCS(X, m, Y, n, result);

    std::cerr<<elpased_seconds.count()<<std::endl;


    return 0;

}
//    om.parfor<std::vector<std::vector<int>>>(0, nbthreads, 1, m, n,
//            [&](std::vector<std::vector<int>> & tls) -> void{
//                for (int i=0; i<=m; ++i) {
//                    std::vector<int> vec;
//                    tls.push_back(vec);
//                    tls[i][0] = 0;
//                }
//                for (int j=0; j<=n; ++j) {
//                    tls[0][j] = 0;
//                }
//            },
//            [&](int a, int b, std::vector<std::vector<int>> &tls) -> void{
//                if (X[a-1] == Y[b-1]) {
//                    tls[a][b] = tls[a-1][b-1] + 1;
//                } else {
//                    tls[a][b] = std::max(tls[a-1][b], tls[a][b-1]);
//                }
//            },
//            [&](std::vector<std::vector<int>> &tls) -> void {
//                result = tls[m][n];
//
////                    for (int i=0; i<=m; ++i) {
////                        delete[] tls[i];
////                    }
////                    delete[] tls;
//            });
//

