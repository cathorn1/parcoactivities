#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <ctime>

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

using std::chrono::system_clock;

int func, points, intensity;
float lower, upper;

int main (int argc, char* argv[]) {

  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }

  sscanf(argv[1], "%d", &func);
  sscanf(argv[2], "%f", &lower);
  sscanf(argv[3], "%f", &upper);
  sscanf(argv[4], "%d", &points);
  sscanf(argv[5], "%d", &intensity); 

  float result = 0.0;
  float x = ((upper-lower)/points);
  float itgr_output = 0.0;
  
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  if (func == 1) {
    for (int i=0; i < (points-1); i++) {
      itgr_output += f1(x, intensity);
    }
  }
  else if (func == 2) {
    for (int i=0; i < (points-1); i++) {
      itgr_output += f2(x, intensity);
    }
  }
  else if (func == 3) {
    for (int i=0; i < (points-1); i++) {
      itgr_output += f3(x, intensity);
    }
  }
  else if (func == 4) {
    for (int i=0; i < (points-1); i++) {
      itgr_output += f4(x, intensity);
    }
  }

  result = x * itgr_output;

  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double, std::ratio<1>> duration = end - start;
  double elapsed_time = duration.count();

  std::cout << result << std::endl;
  std::cerr << elapsed_time << std::endl;
  
  //std::cout << "Test output: " << "result: " << result << " func " << func << " lower: " 
  //  << lower << " Upper: " << upper << " points " << points << " intens " << intensity << std::endl;

    
  return 0;
}
