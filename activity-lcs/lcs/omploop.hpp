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

  template<typename C>
  void parfor (size_t beg, size_t end, size_t increment,
               std::function<void(C&)> before,
               std::function<void(int, C&)> f,
               std::function<void(C&)> after
               ) {


#pragma omp parallel num_threads(nbthread)
    {
      C C_arr;
      before(C_arr);

#pragma omp for schedule(static)

for (int i = beg; i < end; i+=increment){
    f(i, C_arr);
}

#pragma omp critical
      after(C_arr);
    }
  }

};

#endif

//      for (int i=1; i <= ((m+n)-1); i++) {
//          int a = i;
//          nbTaskDiag = a;
//
//          if(i> (m)) {
//              a = (m);
//              count--;
//              nbTaskDiag = count;
//              bCount++;
//          }
//
//          for(int j = 1; j <= nbTaskDiag; j++) {
//
//              //k = i -(j-1);
//
//              int b = j;
//              if(i> (m)) {
//                  b = (j+bCount);
//
//                  if(i == (m+n)-1){
//                      b = m;
//                  }
//              }
//              f(a, b, U, W, C_arr);
//
//              if (a !=0) {
//                  a--;
//              }
//          }
//      }

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