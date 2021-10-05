//Clifton Thorne
//10/5/21

#include <iostream>
#include <thread>
#include <vector>

using namespace std;

void minion(int m) {
  cout << "Hello, I am Minion " << m + 1 << "!" << endl;
}

void overlord(){
  cout << "Hello, Minions!! I am the OVERLORD." << endl;
}

int numMinion;

int main (int argc, char** argv) {
  if (argc < 2) {
    std::cerr<<"usage: "<<argv[0]<<" <nbminions>\n";
    return -1;
  }
  
  sscanf(argv[1], "%d", &numMinion);

  vector<thread> mythreads;
  

  for (int i = 0; i < numMinion; i++){
      thread mythread (minion, i);
      
      mythreads.push_back(move(mythread));
  }
  
  
  for (auto & t : mythreads) {
      if (t.joinable())
        t.join();
      else
        cout << "t is not joinable" << endl;
  }

  thread over_thread (overlord);
  over_thread.join();
  
  return 0;
}
