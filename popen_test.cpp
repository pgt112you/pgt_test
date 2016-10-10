/* 测试popen是否会阻塞 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main() {
  time_t t = time(NULL);
  printf("%ld\n", t);
  char out[100];
  FILE *pf = popen("/usr/home/guangtong/test/popen_test.sh", "r");
  fread(out, sizeof(char), 99, pf);
  printf("== %s\n", out);
  t = time(NULL);
  printf("%ld\n", t);

}
