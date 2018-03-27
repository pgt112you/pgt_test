#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <pthread.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include "rocksdb/db.h"
#include "rocksdb/utilities/backupable_db.h"
#include "rocksdb/utilities/checkpoint.h"
#include "rocksdb/utilities/db_ttl.h"
#include "rocksdb/merge_operator.h"
#include "rocksdb/slice.h"
#include "threadpool.h"


using namespace std;
using namespace rocksdb;


int port = 9112;
int MAX_EVENTS = 10;

typedef struct {
    int i;
    char kbuf[256];
    char vbuf[256];
}trans;


rocksdb::DB* db; 


void *put(void *arg) {
    trans *p = (trans *)arg;
    rocksdb::Status status = db->Put(rocksdb::WriteOptions(), p->kbuf, p->vbuf);
    if (!status.ok()) {
        cout << "insert key " << p->kbuf << " value " << p->vbuf << " failed " << endl;
    }
    else {
        cout << "insert key " << p->kbuf << " value " << p->vbuf << " success " << endl;
    }
    return NULL;
}

void *getscore(void *arg) {
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    cout << "before get default columnfamily" << tv.tv_sec << "." << tv.tv_usec << endl;
    ColumnFamilyHandle* ch = db->DefaultColumnFamily();
    gettimeofday(&tv, &tz);
    cout << "before get score columnfamily" << tv.tv_sec << "." << tv.tv_usec <<  endl;
    const char *result = ch->GetScore();
    gettimeofday(&tv, &tz);
    cout << "after get score columnfamily" << tv.tv_sec << "." << tv.tv_usec <<  endl;
    cout << "score is: " << result << endl;
    size_t num = ch->GetImmNum();
    cout << "level 0 imm num is : " << num << endl;
}


int main() {
    rocksdb::Options options;
    options.create_if_missing = true;
    options.write_buffer_size = 1*1024;
    options.max_write_buffer_number = 6;
    options.min_write_buffer_number_to_merge = 2;
    options.max_bytes_for_level_base = 10000;
    options.max_bytes_for_level_multiplier = 10;
    rocksdb::Status status = rocksdb::DB::Open(options, "/data0/home/guangtong/pgt_test/rocksdb_dir", &db);    

    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr, cliaddr;
    memset(&servaddr, '\0', sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(port);
    bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(sockfd, SOMAXCONN);
    socklen_t cliaddr_len = sizeof(cliaddr);
    trans *p = new trans;

    init_pool();
    create_worker(20);


    int nfds;
    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    int epollfd = epoll_create(10); 
    if (epollfd == -1) {
        perror("epoll_create");
        exit(0);
    }
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
        perror("epoll_ctl: listen_sock");
        exit(0);
    }

    int i;
    int num;
    int ii = 0;
    while(1) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_pwait");
            exit(0);
        }        
        for (i=0; i<nfds; ++i) { 
            int clifd;
            if (events[i].data.fd == sockfd) {        // 有新的连接
                clifd = accept(sockfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
                struct epoll_event ev;
                ev.events = EPOLLIN;
                ev.data.fd = clifd;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, clifd, &ev) == -1) {
                    perror("epoll_ctl: listen_sock");
                }
            }
            else {
                clifd = events[i].data.fd;
            }
            bzero(p, sizeof(trans));
            num = read(clifd, p, sizeof(trans));
            if (num <= 0) {    // 对方关闭了连接，将close这个sock，将这个sock对应的event从eventlist中去掉
                printf("in num < 0\n");
                if (epoll_ctl(epollfd, EPOLL_CTL_DEL, clifd, NULL) == -1) {
                    perror("epoll_ctl: listen_sock"); 
                }
                printf("close fd %d\n", clifd);
                close(clifd);
                continue;
            }
            else if (p->i == 0) {  // 写操作
                //pthread_t tid;
                //pthread_attr_t attr;  
                //pthread_attr_init(&attr);  
                //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);  
                //pthread_create(&tid, NULL, put, (void *)p);
                //pthread_attr_destroy(&attr); 
                push_task(put, (void *)p);
            }
            else if (p->i == 15) {   // 测试自己在rocksdb里面加的GetScore函数
                pthread_t tid;
                pthread_attr_t attr;  
                pthread_attr_init(&attr);  
                pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);  
                pthread_create(&tid, NULL, getscore, NULL);
                pthread_attr_destroy(&attr); 
            }
        }
    }
    free(p);
    close(sockfd);
    return 0;
}
