//Clifton Thorne
//ITCS 3145-001

#include <stdio.h>
#include <unistd.h>

using namespace std;

int main () {

  char hostname[64];
  int err = gethostname(hostname, 64);

  if(err ==0){
    printf("Hello there, World!\nPrinting from %s!\n", hostname);
  }
  else{
    printf("Could not determine host name. Error %d", err);
  }
  
  return 0;

}