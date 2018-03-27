#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#define __USE_GNU 1 
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>


#define LOG_FILE_MODE S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IROTH
#define LOG_FILE_FLAG O_DIRECT|O_WRONLY|O_APPEND|O_CREAT
#define BUFSIZE 4096


void *t1(void *args) {
    struct timeval s_val, e_val;
    int res;
    int fd = open("/data/aaa.txt", LOG_FILE_FLAG, LOG_FILE_MODE);
    void *content;
    int ret = posix_memalign((void **)&content, 4096, BUFSIZE);
    if (ret) {
        perror("posix_memalign failed");
        exit(1);
    }
    memset(content, 97, BUFSIZE);
    int num;
    while (1) {
        gettimeofday(&s_val, NULL);
        num = write(fd, (const void *)content, BUFSIZE); 
        gettimeofday(&e_val, NULL);
        if (num < 0) {
            printf("%d\n", errno);
            break;
        }
        res = (e_val.tv_sec - s_val.tv_sec)*1000000 + (e_val.tv_usec - s_val.tv_usec);
        if (res > 50000) {
            printf("%u at: %d.%d\t process too long %d\n", (unsigned int)pthread_self(), (int)e_val.tv_sec, (int)e_val.tv_usec, res);
            fflush(stdout);
        }
        //struct timespec req;
        //req.tv_sec = 0;
        //req.tv_nsec = 200000;
        //nanosleep(&req, NULL);
    }
    close(fd);
}


void *t2(void *args) {
    int fd = open("aaa.txt", LOG_FILE_FLAG, LOG_FILE_MODE);
    while(1) {  
        fsync(fd);  
        sleep(1);  
    } 
}


int main() {
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, t1, NULL);
    //pthread_create(&tid2, NULL, t2, NULL);
    pthread_join(tid1, NULL);
    //pthread_join(tid2, NULL);
    return 0;
}
