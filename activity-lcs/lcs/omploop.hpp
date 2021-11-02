#ifndef __OMPLOOP__
#define __OMPLOOP__

#include <functional>

// That does not solve the static activity, but provide the same feature so it can be used as a replacement.
// You need to add -fopenmp to CXXFLAGS and LDFLAGS for this to work.
// set number of thread by called setNbThread.
class OmpLoop {

  int nbthread;
public:

  OmpLoop()
    :nbthread(1)
  {}
  
  void setNbThread(int t) {
    nbthread = t;
  }

//  int i = 0;
//  int j = 0;
//  int k = 0;
  int diag= 0;
  int nbDiag= 0;
  int taskDiag =0;
  int nbTaskDiag= 0;
  int count = 0;

  template<typename C>
  void parfor (size_t beg, size_t end, size_t increment, size_t m, size_t n, char* U, char* W,
               std::vector<std::vector<int>>,
               std::function<void(C&)> before,
               std::function<void(int, int, char*, char*, C&)> f,
               std::function<void(C&)> after
               ) {

#pragma omp parallel num_threads(nbthread)
    {
      C C_arr;
      before(C_arr);


#pragma omp for schedule(static)


      for (int i=1; i <= ((m+n)-1); i++) {
          int a = i;
          if(i>= (m-1)) {
              a = (m-1);
          }

              for(int j = 0; j <i; j++) {

                  int b = j+1;
                  f(a, b, U, W, C_arr);
                  a--;
              }
      }
#pragma omp critical
      after(C_arr);
    }
  }

};

#endif

//nbDiag = (m+n)-1;
//
//          for (i=1; i <= m; i++) {
//          //a = diag;
//          //nbTaskDiag = a;
//
////          if(i>= (m-1)) {
////              i = (m-1);
////          }
////          if (nbTaskDiag > m){
////              //HOW DO DETERMINE nbTaskDiag?
////              nbTaskDiag -= m+count;
////              count++;
////          }
//
//          for(j = 1; j <=i; j++) {
//
//              k = i -(j-1);
//              f(i, j, k, U, W, C_arr);
//
//              //i--;
//
//          }
//      }