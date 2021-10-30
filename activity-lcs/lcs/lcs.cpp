#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <array>
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

  if (argc < 4) { std::cerr<<"usage: "<<argv[0]<<" <m> <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int nbthreads = atoi(argv[3]);
  int result;
  OmpLoop om;

  // get string data 
  char *X = new char[m];
  char *Y = new char[n];
  generateLCS(X, m, Y, n);
  //insert LCS code here.

   om.parfor<std::vector<std::vector<int>>>(0, nbthreads, 1, m, n,
           [&](std::vector<std::vector<int>> & tls) -> void{
               for (int i=0; i<=m; ++i) {
                   std::vector<int> vec;
                   tls.push_back(vec);
                   tls[i][0] = 0;
               }
               for (int j=0; j<=n; ++j) {
                   tls[0][j] = 0;
               }
                },
                [&](int a, int b, std::vector<std::vector<int>> &tls) -> void{
                    if (X[a-1] == Y[b-1]) {
                        tls[a][b] = tls[a-1][b-1] + 1;
                    } else {
                        tls[a][b] = std::max(tls[a-1][b], tls[a][b-1]);
                    }
                },
                [&](std::vector<std::vector<int>> &tls) -> void {
                    result = tls[m][n];

//                    for (int i=0; i<=m; ++i) {
//                        delete[] tls[i];
//                    }
//                    delete[] tls;
                });



  //int result = -1; // length of common subsequence


  checkLCS(X, m, Y, n, result);


  return 0;
}
