#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main() {
  char temp[1000];
  printf("time is %ld\n", time(NULL));
  FILE *pf = popen("/usr/home/guangtong/test/popen_test.sh", "r");
  fread(temp, sizeof(char), 10, pf);
  printf("temp is %s\n", temp);
  printf("time is %ld\n", time(NULL));
  printf("===================\n");
  printf("time is %ld\n", time(NULL));
  system("/usr/home/guangtong/test/popen_test.sh");
  printf("time is %ld\n", time(NULL));
  fread(temp, sizeof(char), 100, pf);
  printf("temp is %s\n", temp);
  pclose(pf);
  pf = popen("ping 10.210.128.53", "r");
  pclose(pf);
}
