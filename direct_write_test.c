#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#define __USE_GNU 1 
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <linux/fs.h>
#include <string.h>


#define LOG_FILE_MODE S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IROTH
#define LOG_FILE_FLAG O_DIRECT|O_WRONLY|O_APPEND|O_CREAT
#define LOG_FILE_FLAG1 O_WRONLY|O_APPEND|O_CREAT
#define BUFSIZE 1024 
#define BUFSIZE1 124 
#define BLOCKSIZE 512


int main() {
    int fd = open("aaa.txt", LOG_FILE_FLAG1, LOG_FILE_MODE);
    struct stat fst;
    fstat(fd, &fst);
    int begin_diff = BLOCKSIZE - fst.st_size%BLOCKSIZE;
    printf("file size is %d\tbegin diff size is %d\n", fst.st_size, begin_diff);
    void *buf1;
    void *buf;
    int num;
    buf1 = malloc(begin_diff);
    memset(buf1, 100, begin_diff);
    num = write(fd, buf1, begin_diff);
    if (num < 0) {
        printf("AAA %d\n", errno);
        exit(0);
    }
    close(fd);
    free(buf1);
    /* ---------------------------------------------------------------- */
    
    //int rc = ioctl(fd, BLKGETSIZE, &BLOCKSIZE);
    //if (rc < 0) {
    //    perror("ioctl error");
    //    exit(0);
    //}
    fd = open("aaa.txt", LOG_FILE_FLAG, LOG_FILE_MODE);
    printf("block size is %d\n", BLOCKSIZE);
    int ret = posix_memalign((void **)&buf, BLOCKSIZE, BUFSIZE);
    memset(buf, 97, BUFSIZE);
    num = write(fd, (const void *)buf, BUFSIZE);
    if (num < 0) {
        printf("aaaa %d\n", errno);
        exit(0);
    }
    close(fd);
    free(buf);
    /* ---------------------------------------------------------------- */
    fd = open("aaa.txt", LOG_FILE_FLAG1, LOG_FILE_MODE);
    buf1 = malloc(BUFSIZE1);
    memset(buf, 98, BUFSIZE1);
    num = write(fd, (const void *)buf, BUFSIZE1);
    if (num < 0) {
        printf("%d\n", errno);
        exit(0);
    }
    close(fd);
    free(buf1);
    /* ---------------------------------------------------------------- */
    fd = open("aaa.txt", LOG_FILE_FLAG1, LOG_FILE_MODE);
    int size = BUFSIZE - BUFSIZE1;
    printf("size is %d\n", size);
    buf1 = malloc(size);
    memset(buf1, 99, size);
    num = write(fd, (const void *)buf1, size);
    if (num < 0) {
        printf("%d\n", errno);
        exit(0);
    }
    close(fd);
    free(buf1);
    /* ---------------------------------------------------------------- */
    fd = open("aaa.txt", LOG_FILE_FLAG, LOG_FILE_MODE);
    //rc = ioctl(fd, BLKGETSIZE, &BLOCKSIZE);
    printf("block size is %d\n", BLOCKSIZE);
    ret = posix_memalign((void **)&buf, BLOCKSIZE, BUFSIZE);
    memset(buf, 97, BUFSIZE);
    num = write(fd, (const void *)buf, BUFSIZE);
    if (num < 0) {
        printf("bbbb %d\n", errno);
        exit(0);
    }
    close(fd);
    free(buf);
    /* ---------------------------------------------------------------- */
    fd = open("aaa.txt", LOG_FILE_FLAG1, LOG_FILE_MODE);
    buf1 = malloc(36);
    memset(buf1, 101, 36);
    num = write(fd, (const void *)buf1, 36);
    if (num < 0) {
        printf("%d\n", errno);
        exit(0);
    }
    close(fd);
    free(buf1);

    return 0;
}
