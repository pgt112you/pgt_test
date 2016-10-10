#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>


void *thr_fn(void *arg) {
  int oldstate;
  int num;
  printf("into thr_fn\n");
  num = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldstate);
  printf("num is %d\n", num);
  printf("before sleep\n");
  sleep(5);
  printf("after sleep\n");
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldstate);
  printf("out thr_fn\n");
}


int main() {
  pthread_t tid;
  pthread_create(&tid, NULL, thr_fn, NULL);
  sleep(2);
  printf("it is here0\n");
  pthread_cancel(tid);
  printf("it is here\n");
}
