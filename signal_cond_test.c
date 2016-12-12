#include "pgt_test.h"
#include <pthread.h>


static pthread_cond_t pc = PTHREAD_COND_INITIALIZER;;
static pthread_mutex_t pc_mut = PTHREAD_MUTEX_INITIALIZER;


void *func(void *arg) {
    sleep(3);
    pthread_cond_signal(&pc);
}


int main() {

    pthread_t tid;
    pthread_create(&tid, NULL, func, NULL);
    int i = 0;
    while(1) {
        pthread_mutex_lock(&pc_mut);
        printf("before wait\n", i);
        pthread_cond_wait(&pc, &pc_mut);
        ++i;
        printf("in mutex ++i is %d\n", i);

        pthread_mutex_unlock(&pc_mut);
        pthread_cond_signal(&pc);   // 下面的这三个信号应该不会起作用，因为还没有处于正在等待的线程，所以这个条件信号就等于是丢失了
        pthread_cond_signal(&pc);
        pthread_cond_signal(&pc);
    }





}
