#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif



int main (int argc, char* argv[]) {

  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }

  int func = *argv[0];
  int lower = *argv[1];
  int upper = *argv[2];
  int points = *argv[3];
  int intensity = *argv[4];

  float result = 0.0;
  float x = 0.0;
  
  if (func == 1) {
    for (int i=0; i < (points-1); i++) {
      result = f1(x, intensity);
    }
  }
  else if (func == 2) {

  }
  else if (func == 3) {
    
  }
  else if (func == 4) {
    
  }

  std::cout << "Test output: " << result << std::endl;

  return 0;
}
