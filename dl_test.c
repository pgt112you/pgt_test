#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


void *thr_fn(void *arg) {
  typedef char *(*pfunc)(char *);
  void *dl_handler = dlopen("/usr/home/guangtong/test/dl_test.so", RTLD_NOW);
  pfunc getdata = (pfunc)dlsym(dl_handler, "getdata");
  printf("in fn %s\n", (char *)arg);
  char *res = getdata("/usr/home/guangtong/test/dl_test.sh");
  printf("result is %s\n", res);
  dlclose(dl_handler);
}


int main() {
  pthread_t tid, tid2;
  pthread_create(&tid, NULL, thr_fn, (void *)"fn1");
  pthread_create(&tid2, NULL, thr_fn, (void *)"fn2");
  pthread_join(tid, NULL);
  pthread_join(tid2, NULL);
  return 0;
}
