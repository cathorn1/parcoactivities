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
//    int i =0;
//    int j =0;
//    int k =0;
    int s=0;
    int count =0;
    int Lmax =0;


    for (int i = 0; i <=m; ++i) {
        std::vector<int> vec(1, 0);
        C_a.push_back(vec);
        parent.push_back(0);
        //C_a[i][0] = 0;
        //printf("%s %d\n", "howdy 1", i);
    }
    for (int j = 0; j <=n; ++j) {
        //printf("%s %d\n", "howdy 2", j);
        C_a[0][j] = 0;
    }

    om.parfor<std::vector<std::vector<int>>>(0, m, 1, m, n, X, Y, std::ref(C_a),
            [&](std::vector<std::vector<int>> &C) -> void {
                //C = C_a;
            },
            [&](int i, int j, int k, char* U, char* W, std::vector<std::vector<int>> &C) -> void {
                printf("%s, i: %d, j: %d, k: %d\n", "howdy 3", i, j, k);
                printf("X size: %ld, Y size: %ld\n", sizeof(X), sizeof(Y));

                if ((k-1) < sizeof(X) && (j-1 < sizeof Y)) {
                    if (X[k - 1] == Y[j - 1]) {
                        printf("%s\n", "howdy 4");
                        C_a[k][j] = (C_a[k - 1][j - 1]) + 1;
                        {

                            if (C_a[k][j] > Lmax) {
                                printf("%s\n", "howdy 5");
                                Lmax = C_a[k][j];
                                parent[s] = k - 1;
                                s++;
                                count++;
                            }
                        }
                    } else if (C_a[k - 1][j] >= C_a[k][j - 1]) {
                        printf("%s\n", "howdy 6");
//                    printf("tls: %d\n", tls[a][b]);

                        C_a[k][j] = C_a[k - 1][j];

                    } else {
                        printf("%s\n", "howdy 7");
                        C_a[k][j] = C_a[k][j - 1];
                        //C_a[k][j] = std::max(C_a[k - 1][j], C_a[k][j - 1]);
                    }
                    printf("%s %d\n", "from middle ", C_a[k][j]);

                }
            },
            [&](std::vector<std::vector<int>> &C) -> void {

//                printf("%s\n", "howdy 6");
                //answer = C_a[m][n];
//                printf("%s %d\n", "from last ", tls[m][n]);
//                printf("%s\n", "howdy 77");

//                    for (int i=0; i<=m; ++i) {
//                        delete tls[i];
//                    }
//                    delete tls;
            });

    //int result = answer; // length of common subsequence
    printf("%s\n", "made bottom");
    printf("%s %d\n", "from end", answer);

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> elpased_seconds = end-start;

    checkLCS(X, m, Y, n, answer);

    std::cerr<<elpased_seconds.count()<<std::endl;
    std::cout<<answer<<std::endl;

    return 0;

}


//                for (i = 1; i <= m; i++){
//                    for (j =1; j<=i; j++){
//                        k = i-(j-1);
//
//                        if(X[k-1]==Y[j-1]){
//                            C[k][j]=C[k-1][j-1]+1; {
//                                if(C[k][j]>LCSmax){
//                                    LCSmax =C[k][j];
//                                    //parent[s]=k-1;
//                                    s++;
//                                    count++;
//                                }
//                            }
//
//                        }
//                        else if(C[k-1][j] >= C[k][j-1]){
//                            C[k][j]=C[k-1][j];
//                        }
//                        else{
//                            C[k][j]=C[k][j-1];
//                        }
//                    }
//                }


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

