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

void LCS (int a, int b, int m, int n, char* U, char* W, int** C_arr) {

    if (U[a - 1] == W[b - 1]) {
        //printf("%s\n", "howdy 4");
        C_arr[a][b] = (C_arr[a - 1][b - 1]) + 1;
    }
    else{
        C_arr[a][b] = std::max(C_arr[a - 1][b], C_arr[a][b - 1]);
    }
    //return C_arr[a][b];
}

int main (int argc, char* argv[]) {

    if (argc < 4) {
        std::cerr << "usage: " << argv[0] << " <m> <n> <nbthreads>" << std::endl;
        return -1;
    }

    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int nbthreads = atoi(argv[3]);

    int answer = 0;
    OmpLoop om;

    // get string data
    char *X = new char[m];
    char *Y = new char[n];
    generateLCS(X, m, Y, n);
    //insert LCS code here.

    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    om.setNbThread(nbthreads);

    std::vector<std::vector<int>> C_a;
    std::vector<int> parent;

    int **C_arr = new int *[m + 1];

    for (int i = 0; i <= m; ++i) {
        C_arr[i] = new int[n + 1];
        C_arr[i][0] = 0;
    }
    for (int j = 0; j <= n; ++j) {
        C_arr[0][j] = 0;
    }

    om.parfor<int>(0, nbthreads, 1,
            [&](int &C) -> void {

            },
            [&](int i, int &C) -> void {
                for(int a =1; a <= m; a++){
                    int diagA =a;
                    for(int b =1; b<=n; b++){
                        LCS(diagA, b, m, n, X, Y, std::ref(C_arr));
                        diagA--;
                        if(diagA < 1)
                            break;
                    }
                }

                for (int b = 2; b <=n; b++){
                    int diagB = b;
                    for(int a =m; a > 0; a--){
                        LCS(a, diagB, m, n, X, Y, std::ref(C_arr));
                        diagB++;
                        if(diagB > n)
                            break;
                    }
                }
            },
            [&](int &C) -> void {
                    C =0;
            });

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> elpased_seconds = end-start;

    int result = C_arr[m][n];
    checkLCS(X, m, Y, n, result);

    std::cerr<<elpased_seconds.count()<<std::endl;
    //std::cout<<answer<<std::endl;

    return 0;

}

//om.parfor< std::vector < std::vector < int>>>(0, end, 1, m, n, X, Y,
//            [&](std::vector <std::vector<int>> &tls) -> void {
//
//            },
//            [&](int a, int b, char* U, char* W, std::vector <std::vector<int>> &tls) -> void {
////                printf("%s\n", "howdy 3");
////                printf("%d\n", tls[a][b]);
//
//                if (U[a - 1] == W[b - 1]) {
////                    printf("%s a: %d b: %d\n", "howdy 4", a, b);
//                    tls[a][b] = (tls[a - 1][b - 1]) + 1;
//                } else {
////                    printf("%s a: %d b: %d\n", "howdy 5", a, b);
////                    printf("tls: %d\n", tls[a][b]);
//
//                    tls[a][b] = std::max(tls[a - 1][b], tls[a][b - 1]);
//                }
//                //printf("%s %d\n", "from middle ", tls[m][n]);
//
//            },
//            [&](std::vector <std::vector<int>> &tls) -> void {
//
////                printf("%s\n", "howdy 6");
//                answer = tls[m][n];
////                printf("%s %d\n", "from last ", tls[m][n]);
////                printf("%s\n", "howdy 77");
//
////                    for (int i=0; i<=m; ++i) {
////                        delete tls[i];
////                    }
////                    delete tls;
//            });

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

