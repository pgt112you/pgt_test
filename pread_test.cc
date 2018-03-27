#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>


#define BLOCKSIZE 4096

typedef struct {
    const char **filename;
    int len;
} targ;

typedef struct {
    const char *fname;   // file name
    int fd;              // file fd
    int offset;          // read offset
    int fsize;           // file size
} readinfo;


bool pread_fn(int fd, int *offset, char *buff, const char* filename) {
    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);
   
    int res = pread(fd, buff, BLOCKSIZE+5, *offset);
    if (res < 0) {
        printf("read file %s error: %s\n", filename, strerror(errno));
        return false;
    } 
   
    gettimeofday(&tv2, NULL);
    int diff = (tv2.tv_sec - tv1.tv_sec) * 1000 * 1000;
    diff += (tv2.tv_usec - tv1.tv_usec);
    if (diff > 50*1000) {
        char tmbuf[64];
        struct tm *tm1 = localtime(&tv2.tv_sec);
        strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d %H:%M:%S", tm1);
        printf("%s.%6ld %ld read block read(pread) from file more than 50ms %d res %d\n", tmbuf, tv2.tv_usec, pthread_self(), diff/1000, res);
    }
    
    posix_fadvise(fd, *offset, res, POSIX_FADV_DONTNEED);
    *offset = *offset + res;
    return true;
}

int open_fn(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        printf("open file %s error: %s\n", filename, strerror(errno));
        return -1;
    }
    return fd;
}

bool stat_fn(int fd, struct stat* info, const char *filename) {
    if (fstat(fd, info) < 0) {
        printf("get file %s stat error: %s\n", filename, strerror(errno));
        return false;
    }
    return true;

}

readinfo *init_fn(const char *fname) {
    int fd = open_fn(fname);
    if (fd < 0) {
        return NULL;
    }
    struct stat finfo;
    if (!stat_fn(fd, &finfo, fname)) {
        return NULL;
    }
    readinfo *rinfo = new(readinfo);
    rinfo->fname = fname;
    rinfo->fd = fd;
    rinfo->offset = 0;
    rinfo->fsize = finfo.st_size;
    return rinfo;
}

void *t_fn(void *arg) {
    targ * t= (targ *)arg;
    const char **filename = t->filename;
    int len = t->len;
    readinfo *rinfo_list[len];
    int i = 0;
    printf("len is %d\n", len);
    for (; i<len; i++) {
        printf("filename is %s, i is %d\n", *(filename+i), i);
        rinfo_list[i] = init_fn(*(filename+i));
        if(rinfo_list[i] == NULL) {
            return NULL;
        }
    }

    char buff[BLOCKSIZE*2];
    while(1) {
        i = rand() % len;
        if (!pread_fn(rinfo_list[i]->fd, &rinfo_list[i]->offset, buff, rinfo_list[i]->fname)) {
            break;
        }
        if (rinfo_list[i]->offset > rinfo_list[i]->fsize-(BLOCKSIZE*5)) {
            rinfo_list[i]->offset = 0;
        }
    }
    return NULL;
}

int main() {
    const char *filename1 = "/data/aaa.txt";
    const char *filename2 = "/data/aab.txt";
    const char *filename3 = "/data/abc.txt";
    const char *filename[3] = {filename1, filename2, filename3};
    targ t;
    t.filename = filename;
    t.len = 3;
    pthread_t tid;
    pthread_create(&tid, NULL, t_fn, (void*)&t);
    pthread_join(tid, NULL);
    return 0;
}
