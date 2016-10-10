/*
 * 测试跨线程释放内存是否能行
 * 结论是能释放，最少看起来能释放
*/

#include "pgt_test.h"

char *mem1;
char *mem2;

void *thr_fn1(void *arg) {
  free(mem1);
  free(mem2);
  sleep(2);
  return NULL;
}

void *thr_fn2(void *arg) {
  mem2 = (char *)malloc(sizeof(char)*25);
  memset(mem2, '\0', sizeof(char)*25);
  strcpy(mem2, "jia jie shi\n");
  sleep(2);
  printf("in thr_fn2, mem2 is %s\n", mem2);
  return NULL;
}


int main() {
  pthread_t tid1, tid2;
  mem1 = (char *)malloc(sizeof(char)*25);
  memset(mem1, '\0', sizeof(char)*25);
  strcpy(mem1, "hello world\n");
  pthread_create(&tid2, NULL, thr_fn2, NULL);
  sleep(1);
  pthread_create(&tid1, NULL, thr_fn1, NULL);
  sleep(1);
  printf("mem1 content is %s\n", mem1);
  printf("mem2 content is %s\n", mem1);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
}
