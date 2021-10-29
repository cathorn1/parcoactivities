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

//  template<typename TLS>
//  void parfor (size_t beg, size_t end, size_t increment,
//	       std::function<void(TLS&)> before,
//	       std::function<void(int, TLS&)> f,
//	       std::function<void(TLS&)> after
//	       ) {
//    TLS tls;
//    before(tls);
//    for (size_t i=beg; i<end; i+= increment) {
//      f(i, tls);
//    }
//    after(tls);
//  }
//
//};



//template<typename TLS>
//
//void parfor (size_t beg, size_t end, size_t increment, size_t n,
//             std::function<void(TLS&)> before,
//             std::function<void(int, int, TLS&)> f,
//             std::function<void(TLS&)> after
//) {
//
//    TLS tls;
//    before(tls);
//    int inc =1;
//    int itrs = n/end;
//    int remain = n%end;
//    for (size_t i=beg; i<end; i+= increment) {
//        int up = itrs * inc;
//        int low = up - itrs;
//        up -= 1;
//        if (beg + 1 == end){
//            up += remain;
//        }
//        inc++;
//        std::thread t(f, low, up, std::ref(tls));
//        t.join();
//    }
//    after(tls);
//}
//
//};

    void helpMe(std::function<void(int, int, std::vector<double>&)> f, double lower, double upper, int chunk, int chunkRemain, std::vector<double> &tls, std::vector<std::thread> &tVec){
        while(true){
            tVec.push_back(std::thread(f, lower, upper, std::ref(tls)));
        }
    }


template<typename TLS>
void parfor (size_t beg, size_t end, size_t increment, size_t n, size_t gran,
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
    int chunkSize = n / gran;
    int chunkRemain = n % gran;

    std::vector <std::thread> tVec;

    int up, low;

    for(int i = beg; i < end; i++) {
        //printf("%s \n", "seg fault 2");
        up = chunkSize * inc;
        low = up - chunkSize;
        up -= 1;
        if (counter + 1 == chunkSize) {
            up += chunkRemain;
        }
        printf("low: %d, up: %d, inc: %d, chunkSize: %d, chunkRemain: %d, count: %d \n", low, up, inc, chunkSize, chunkRemain, counter);

        tVec.push_back(std::thread(f, low, up, counter, std::ref(tls)));

        inc++;
        counter++;
    }
//        size_t k = beg;
//        bool cont = true;
//        std::mutex mut;

//        while (cont == true) {
//            mut.lock();
//            size_t localk = k;
//            k += chunkSize;
//            if (localk < end)
//                cont = false;
//            mut.unlock();
//            f(low, k, counter, std::ref(tls));
//
//        }


        for(auto &t : tVec){
            //printf("%s \n", "seg fault 4");
            t.join();
    }
    //printf("%s \n", "seg fault 5");
    after(tls);
}

};


#endif
