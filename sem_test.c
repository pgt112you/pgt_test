#include "pgt_test.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/syscall.h>


int main() {
    int fdin, fdout;
    void *src, *dst;
    struct stat statbuf;
    int i = 5;
    sem_t sem;

    if ((src = mmap(NULL, sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0)) == MAP_FAILED) {
        printf("mmap error %s\n", strerror(errno));
        exit(0);
    }
    sem_init((sem_t *)src, 1, 11);
    printf("sem is %ld\n", *(long *)src);

    int pid = fork();
    if (pid == 0) {     // child
        sleep(1);
    }
    else {
        sleep(3);
    }

    if (pid == 0) {    // child
        while(sem_wait((sem_t *)src) == 0) {
            printf("in process sem is %ld\n", *(long *)src);
            sleep(1);
        }
    }
    else {
        while(sem_wait((sem_t *)src) == 0) {
            printf("2222 in process sem is %d\n", *(long *)src);
            sleep(1);
        }
    }

    printf("in process %d memory i is %ld\n",getpid(), *(long *)src);
    return 0;

}
