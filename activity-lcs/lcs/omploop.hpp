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

  template<typename TLS>
  void parfor (size_t beg, size_t end, size_t increment, size_t m, size_t n, char* U, char* W,
               std::function<void(TLS&)> before,
               std::function<void(int, int, char*, char*, TLS&)> f,
               std::function<void(TLS&)> after
               ) {
#pragma omp parallel num_threads(nbthread)
    {
      TLS tls;
      before(tls);
      
#pragma omp for schedule(static)

      for (size_t i=1; i<((m*n)); i+= increment) {
          //f(tls);
          int a = (i/n) + 1;
          int b = (i%n) + 1;

          f(a, b, U, W, tls);
      }
#pragma omp critical
      after(tls);
    }
  }

};

#endif
