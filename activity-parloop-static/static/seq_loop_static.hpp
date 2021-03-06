#ifndef __SEQ_LOOP_H
#define __SEQ_LOOP_H

#include <functional>
#include <thread>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <future>
#include <array>

class SeqLoop {
public:
  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel

  void parfor (size_t beg, size_t end, size_t inc,
	       std::function<void(int)> f) {
    for (size_t i=beg; i<end; i+= inc) {
      f(i);
    }
  }


  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel.
  ///
  /// Each thread that participate in the calculation will have its
  /// own TLS object.
  ///
  /// Each thread will execute function before prior to any
  /// calculation on a TLS object.
  ///
  /// Each thread will be passed the TLS object it executed before on.
  ///
  /// Once the iterations are complete, each thread will execute after
  /// on the TLS object. No two thread can execute after at the same time.



template<typename TLS>
void parfor (size_t beg, size_t end, size_t increment, size_t n,
             std::function<void(TLS&)> before,
             std::function<void(int, int, int, TLS&)> f,
             std::function<void(TLS&)> after
) {

    TLS tls;
    before(tls);

    int inc = 1;
    int counter = 0;
    int vecIndex =0;
    int itrs = n / end;
    int remain = n % end;
    int tNum = end;

    std::vector <std::thread> tVec;


       for (int i= beg; i<end; i++) {

           int up = itrs * inc;
           int low = up - itrs;
           up -= 1;
           if (i + 1 == end){
               up += remain;
           }

           //printf("low: %d, up: %d, inc: %d, itrs: %d, remain: %d, count: %d \n", low, up, inc, itrs, remain, counter);

           tVec.push_back(std::thread(f, low, up, counter, std::ref(tls)));

           counter++;
           inc++;
        }

        for(auto &t : tVec){
            t.join();
        }

        after(tls);
    }

};

#endif
