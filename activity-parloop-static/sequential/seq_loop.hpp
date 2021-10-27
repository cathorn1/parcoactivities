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

template<typename TLS>
void parfor (size_t beg, size_t end, size_t increment, size_t n, size_t gran,
             std::function<void(TLS&)> before,
             std::function<void(int, int, TLS&)> f,
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

    std::vector <std::thread> tVec(tNum);
    std::vector <std::future<void>> finished;
    int up, low;

    std::mutex mu;

           while(counter < n) {

            up = chunkSize * inc;
            low = up - chunkSize;
            up -= 1;
            if (counter + 1 == chunkSize) {
                up += chunkRemain;
            }

            //std::thread theThread(f, low, up, std::ref(tls));
//
//            tVec.push_back(std::move(theThread));

           finished.push_back(std::move(std::async(std::launch::async, f, low, up, std::ref(tls))));

            inc++;
            counter += chunkSize;

            for(auto &f : finished){
               f.get();
           }
    }


//    for (auto &t : tVec){
//        if(t.joinable())
//            t.join();
//        else
//            continue;




//    for (size_t i = beg; i < end; i += increment) {
//        up = chunkSize * inc;
//        low = up - chunkSize;
//        up -= 1;
//        counter += chunkSize;
//        inc++;
//
//        std::thread theThread(f, low, up, std::ref(tls));
//
//        while (counter < n) {
//            mu.lock();
//            low = counter;
//            up = counter + chunkSize - 1;
//            counter += chunkSize;
//            if (up > n) {
//                up = n;
//                counter = n;
//            }
//            f(low, up, std::ref(tls));
//
//            mu.unlock();
//        }
//            theThread.join();
//    }



//    for (int k =0; k<end; k++){
//        tVec[k] = std::thread();
//    }
//
//    while(counter < n) {
//
//        if (tVec[vecIndex].joinable()) {
//            vecIndex++;
//            if (vecIndex > 3)
//                vecIndex = 0;
//        }
//        else if (!(tVec[vecIndex].joinable())) {
//            up = chunkSize * inc;
//            low = up - chunkSize;
//            up -= 1;
//            if (counter + 1 == chunkSize) {
//                up += chunkRemain;
//            }
//
//            tVec.push_back(std::move(std::thread(f, low, up, std::ref(tls))));
//
//            inc++;
//            counter += chunkSize;
//        }
//        vecIndex++;
//    }
//    for(auto &t : tVec)
//            t.join();



//    if (tVec.size() != 0) {
//        for(auto &t : tVec)
//            if (t.joinable())
//              t.join();
//    }

    after(tls);
}

};

#endif
