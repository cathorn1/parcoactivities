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

//  int a = 0;
//  int b = 0;

  template<typename C>
  void parfor (size_t beg, size_t end, size_t increment, size_t m, size_t n, char* U, char* W,
               std::vector<std::vector<int>>,
               std::function<void(C&)> before,
               std::function<void(C&)> f,
               std::function<void(C&)> after
               ) {

#pragma omp parallel num_threads(nbthread)
    {
      C C_arr;
      before(C_arr);
      
#pragma omp for schedule(static)

      f(C_arr);

//      for (size_t i=0; i < ((m+n)-1); i+= increment) {
//          //f(tls);
//          int a = ;
//          int b = ;
//
//          f(a, b, U, W, C_arr);
//      }
#pragma omp critical
      after(C_arr);
    }
  }

};

#endif
