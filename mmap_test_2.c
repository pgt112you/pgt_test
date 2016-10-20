#include "pgt_test.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>


int main() {
    int fdin, fdout;
    void *src, *dst;
    struct stat statbuf;
    int i = 5;

    if ((src = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0)) == MAP_FAILED) {
        printf("mmap error %s\n", strerror(errno));
        exit(0);
    }

    int pid = fork();
    if (pid == 0) {     // child
        sleep(1);
    }
    else {
        sleep(3);
    }

    if (pid == 0) {    // child
        i = i + 5;
        int tempi;
        memcpy(src, &i, sizeof(i));
        memcpy(&tempi, src, sizeof(i));
        printf("in process tempi is %d\n", tempi);
    }
    else {
        int tempi;
        memcpy(&tempi, src, sizeof(i));
        i = tempi + 2;
        printf("1111 in process tempi is %d\n", tempi);
        memcpy(src, &i, sizeof(i));
        memcpy(&tempi, src, sizeof(i));
        printf("2222 in process tempi is %d\n", tempi);

    }

    printf("in process %d memory i is %d\n",getpid(), *(int *)src);
    return 0;

}
