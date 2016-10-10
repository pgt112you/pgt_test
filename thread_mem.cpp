/*
 * 测试跨线程释放内存是否能行
*/

#include "pgt_test.h"

char *mem1;
char *mem2;

void *thr_fn1(void *arg) {
  delete mem1;
  delete mem2;
  sleep(2);
  return NULL;
}

void *thr_fn2(void *arg) {
  mem2 = new char;
  *mem2 = 'h';
  sleep(2);
  printf("in thr_fn2, mem2 is %s\n", mem2);
  return NULL;
}


int main() {
  pthread_t tid1, tid2;
  mem1 = new char;
  *mem1 = 'o';
  pthread_create(&tid2, NULL, thr_fn2, NULL);
  sleep(1);
  pthread_create(&tid1, NULL, thr_fn1, NULL);
  sleep(1);
  printf("mem1 content is %c\n", mem1);
  printf("mem2 content is %c\n", mem1);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
}
