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
    char buf[256];
    bzero(buf, 256);

    int pid = fork();
    if (pid == 0) {     // child
        sleep(1);
    }
    else {
        sleep(3);
    }


    if ((fdin = open("mmap_test_src.dat", O_RDWR)) < 0) {
        printf("can't open mmap_test_src.dat\n");
        exit(0);
    }

    if (fstat(fdin, &statbuf) < 0) {
        printf("fstat error\n");
        exit(0);
    }

    if (pid == 0) {    // child
        //if ((src = mmap(0, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fdin, 0)) == MAP_FAILED) {
        if ((src = mmap(0, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fdin, 0)) == MAP_FAILED) {
            printf("mmap error");
            exit(0);
        }
        memcpy(buf, src, statbuf.st_size);
        printf("%d before write content is %s\n", getpid(), buf);
        memcpy(src, "SKYACTIV Technology", 12);
        bzero(buf, 256);
        memcpy(buf, src, statbuf.st_size);
        printf("%d after write content is %s\n", getpid(), buf);
    }
    else {
        if ((src = mmap(0, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fdin, 0)) == MAP_FAILED) {
            printf("mmap error");
            exit(0);
        }
        memcpy(buf, src, statbuf.st_size);
        printf("%d before write content is %s\n", getpid(), buf);
        //memcpy(src, "hello world", 12);
        bzero(buf, 256);
        //memcpy(buf, src, statbuf.st_size);
        //printf("%d after write content is %s\n", getpid(), buf);

    }

    return 0;

}
