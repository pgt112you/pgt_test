/*
 * 测试多线程中select的阻塞是
 * 值阻塞一个线程还是
 * 所有的线程都阻塞
 *
 * 结论是只阻塞select所在的线程
 * 
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <pthread.h>

void *thr_fn1(void *arg) {
  struct timeval tv;
  //tv.tv_sec = 5;
  tv.tv_sec = 50;
  tv.tv_usec = 0;
  printf("%ld before now time is %d\n", (int)pthread_self(), time(NULL));
  select(1, NULL, NULL, NULL, &tv);
  printf("%ld after now time is %d\n", (int)pthread_self(), time(NULL));
  return;
}

void *thr_fn2(void *arg) {
  printf("%ld now time is %d\n", (int)pthread_self(), time(NULL));
  struct timeval tv;
  //tv.tv_sec = 3;
  tv.tv_sec = 30;
  tv.tv_usec = 0;
  printf("%ld before now time is %d\n", (int)pthread_self(), time(NULL));
  select(1, NULL, NULL, NULL, &tv);
  printf("%ld now time is %d\n", (int)pthread_self(), time(NULL));
  return;
}

int main() {
  pthread_t tid1, tid2;
  pthread_create(&tid1, NULL, thr_fn1, NULL);
  sleep(1);
  pthread_create(&tid2, NULL, thr_fn2, NULL);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  return 0;
}
