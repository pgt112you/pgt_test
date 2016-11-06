#include <pthread.h>
#include "pgt_test.h"


void *func(void *arg) {
    printf("in thread %ld\n", pthread_self());
    sleep(500);
}


int main() {
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, func, NULL);
    pthread_create(&tid2, NULL, func, NULL);
    pthread_create(&tid3, NULL, func, NULL);

    sleep(5);
    
    int pid = fork();

    printf("111 pid is %d\n", getpid());
    if (pid > 0) {
        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);
        pthread_join(tid3, NULL);
    }

    printf("before pid is %d\n", getpid());
    sleep(500);
    printf("pid is %d\n", getpid());

    if (pid != 0) {
        wait(NULL);
        printf("child process end\n");
    }

    return 0;

}
