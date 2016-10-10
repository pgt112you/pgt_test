#include "pgt_test.h"


void *thr_fn(void *arg) {
  int res = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
  if (res != 0) {
    printf("err: %s\n", strerror(res));
  }
  sleep(1);
  int i = 100;
  while (i-- > 0) {
    printf("%d\n", i);
  }
  sleep(2);
  printf("it is over\n", i);
  return NULL;
}


int main() {
  pthread_t tid1, tid2;
  pthread_attr_t attr;
  int err = pthread_attr_init(&attr);
  if (err != 0) {
    return -1;
  }
  err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  if (err != 0) {
    pthread_attr_destroy(&attr);
    return -1;
  }
  pthread_create(&tid1, &attr, thr_fn, NULL);
  sleep(2);
  pthread_cancel(tid1);
  sleep(5);

}
