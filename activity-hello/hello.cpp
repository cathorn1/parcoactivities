#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main () {

  char hostname[64];
  int err = gethostname(hostname, 64);

  printf("Hello there, World!\nPrinting from %s!\n", hostname);
  return 0;
}
