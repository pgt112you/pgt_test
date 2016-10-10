#include <stdlib.h>
#include <stdio.h>


int main() {
  printf("time is %d\n", time(NULL));
  system("sleep 2");
  printf("time is %d\n", time(NULL));
}
