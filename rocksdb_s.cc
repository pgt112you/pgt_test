#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <iostream>
#include <unistd.h>
#include "rocksdb/db.h"
#include "rocksdb/utilities/backupable_db.h"
#include "rocksdb/slice.h"


using namespace std;
using namespace rocksdb;


int port = 9112;
int MAX_EVENTS = 10;

typedef struct {
    int i;
    char kbuf[256];
    char vbuf[256];
}trans;




int main() {
    rocksdb::DB* db; 
    rocksdb::Options options;
    options.create_if_missing = true;
    options.write_buffer_size = 24*1024;
    rocksdb::Status status = rocksdb::DB::Open(options, "/data0/home/guangtong/pgt_test/rocksdb_dir", &db);    
    BackupEngine* backup_engine = NULL;
    


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

            printf("num is %d\n", num);
            // 操作rocksdb
            if (p->i == 0) {  // 写操作
                status = db->Put(rocksdb::WriteOptions(), p->kbuf, p->vbuf);
                if (!status.ok()) {
                    cout << "insert key " << p->kbuf << " value " << p->vbuf << " failed " << endl;
                }
                else {
                    cout << "insert key " << p->kbuf << " value " << p->vbuf << " success " << endl;
                }
            }
            else if (p->i == 1) { // 读操作
                string vv;
                status = db->Get(rocksdb::ReadOptions(), p->kbuf, &vv);
                if (!status.ok()) {
                    cout << "get key " << p->kbuf << " failed " << endl;
                }
                else {
                    cout << "get key " << p->kbuf << " value " << vv << " success " << endl;
                }
            }
            else if (p->i == 2) {  // backup操作
                BackupEngine::Open(Env::Default(), BackupableDBOptions("/data0/home/guangtong/pgt_test/rocksdb_dir_backup"), &backup_engine);
                backup_engine->CreateNewBackup(db);
            }
            else if (p->i == 3) {  // 得到备份信息
                if (backup_engine == NULL) {
                    BackupEngine::Open(Env::Default(), BackupableDBOptions("/data0/home/guangtong/pgt_test/rocksdb_dir_backup"), &backup_engine);
                }
                std::vector<BackupInfo> backup_info;
                backup_engine->GetBackupInfo(&backup_info);
                std::vector<BackupInfo>::iterator it;
                for (it = backup_info.begin(); it != backup_info.end(); it++) {
                    //cout << "backup info " << it->backup_id << " " << it->timestamp << " " <<　it->size << " " << it->number_files << endl;
                    printf("backup info %d %d %d %d\n", it->backup_id, it->timestamp, it->size, it->number_files);
                }
            }
            else if (p->i == 4) {  // 通过backup恢复
                BackupEngineReadOnly* backup_engine;
                status = BackupEngineReadOnly::Open(Env::Default(), BackupableDBOptions("/data0/home/guangtong/pgt_test/rocksdb_dir_backup"), &backup_engine);
                const char *backdir = "/data0/home/guangtong/pgt_test/rocksdb_backup1";
                int backid = atoi(p->vbuf);
                backup_engine->RestoreDBFromBackup(backid, backdir, backdir);
                delete backup_engine;
            }
            else if (p->i == 5) {    // 按照范围删除
                status = db->DeleteRange(rocksdb::WriteOptions(), NULL, p->kbuf, p->vbuf);
                if (!status.ok()) {
                    cout << "delete range error " << status.ToString() << endl;
                }

            }
            else if (p->i == 6) {    // seek and get
                cout << "seek key " << p->kbuf << endl;
                rocksdb::Iterator* it = db->NewIterator(rocksdb::ReadOptions());
                it->Seek(p->kbuf);
                if (!it->Valid()) {
                    cout << "seek to " << p->kbuf << "failed" << endl;
                }
                cout << it->key().ToString() << " : " << it->value().ToString() << endl;
                delete it;

            }
            printf("recv is %s\n", p->kbuf);
            //string out;
            //db->GetProperty("rocksdb.cur-size-all-mem-tables", &out);
            //cout << "memtable size is " << out << endl;
            //cout << "set memtable size is " << options.write_buffer_size << endl;
        }
    }
    free(p);
    close(sockfd);
    return 0;
}
