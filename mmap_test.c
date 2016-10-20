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

    int pid = fork();
    if (pid == 0) {     // child
        sleep(1);
    }
    else {
        sleep(3);
    }


    if ((fdin = open("mmap_test_src.dat", O_RDONLY)) < 0) {
        printf("can't open mmap_test_src.dat\n");
        exit(0);
    }

    if ((fdout = open("mmap_test_dst.dat", O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH)) < 0) {
        printf("can't create mmap_test_dst.dat\n");
        exit(0);
    }

    if (fstat(fdin, &statbuf) < 0) {
        printf("fstat error\n");
        exit(0);
    }
    if (lseek(fdout, statbuf.st_size-1, SEEK_SET) == -1) {
        printf("lseek error %s\n", strerror(errno));
        exit(0);
    }
    if (write(fdout, "", 1) == -1) {
        printf("write error\n");
        exit(0);
    }

    if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0 )) == MAP_FAILED) {
        printf("mmap error");
        exit(0);
    }
    if ((dst = mmap(0, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fdout, 0 )) == MAP_FAILED) {
    //if ((dst = mmap(0, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fdout, 0 )) == MAP_FAILED) {
        printf("%d mmap error", getpid());
        exit(0);
    }

    memcpy(dst, src, statbuf.st_size);
    return 0;

}
