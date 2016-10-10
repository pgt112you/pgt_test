/*
 * 测试重复unlock的情况
 * 看看会不会coredump
*/

#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>


pthread_mutex_t tlock = PTHREAD_MUTEX_INITIALIZER;

void *thr_fn(void *arg) {
  pthread_mutex_lock(&tlock);
  printf("in child thread\n");
  pthread_mutex_unlock(&tlock);
  return (void *)1;
}


int main() {
  pthread_t tid;
  pthread_create(&tid, NULL, thr_fn, NULL);
  sleep(1);
  pthread_join(tid, NULL);
  pthread_mutex_unlock(&tlock);
  return 0;

}
