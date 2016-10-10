/*
 * 测试scanf如果四个字段，只给三个参数，
 * 这样能不能正确赋值
*/
#include "pgt_test.h"


int main() {
  const char *ss = "hello world 112  jiajieshi, gaolujie";
  char *s1 = (char *)malloc(128);
  char *s2 = (char *)malloc(128);
  int i;
  sscanf(ss, "%s %s %d", s1, s2, &i);
  printf("s1 is %s\n", s1);
  printf("s2 is %s\n", s2);
  printf("i is %d\n", i);

}
