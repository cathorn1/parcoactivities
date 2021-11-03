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
  void parfor (size_t beg, size_t end, size_t increment,
               std::function<void(TLS&)> before,
               std::function<void(int, int, TLS&)> f,
               std::function<void(TLS&)> after
               ) {
#pragma omp parallel num_threads(nbthread)
    {
      TLS tls;
      before(tls);
      
#pragma omp for schedule(static) 
          for(int k = 0; k < (end-2); k++) {
              if (k % 2 == 0) {
                  for (int i = 0; i <= (end / 2) - 1; i++) {
                      int a = 2*i;
                      int b = (2*i)+1;
                      if (tls[a] > tls[b])
                          //swap(std::ref(arr), (2*i), ((2*i)+1));
                          f(a, b, tls);
                  }
              } else {
                  for (int i = 0; i <= (end / 2) - 2; i++) {
                      int a =(2 * i) + 1;
                      int b =(2 * i) + 2;
                      if (tls[a] > tls[b])
                          //swap(std::ref(arr), ((2 * i)+1), ((2 * i)+2));
                          f(a, b, tls);

                  }
              }
          }
//          for (size_t i=beg; i<end; i+= increment) {
//	        f(i, tls);
//         }
#pragma omp critical
      after(tls);
    }
  }

};

#endif
