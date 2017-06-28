#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include "rocksdb/db.h"
#include "rocksdb/utilities/backupable_db.h"
#include "rocksdb/utilities/checkpoint.h"
#include "rocksdb/utilities/db_ttl.h"
#include "rocksdb/merge_operator.h"
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



class MergeTestOperator : public AssociativeMergeOperator {
  public:
    virtual bool Merge(const Slice& key, const Slice* existing_value, const Slice& value, std::string* new_value, Logger* logger) const override {
        string *ps = new(string);
        if (existing_value && !existing_value->empty()) {
            ps->append(existing_value->data());
            cout << "existed value is " << existing_value->data() << endl;
        }
        ps->append(value.data());
        cout << "key is " << key.data() << endl;
        cout << "value is " << value.data() << endl;

        new_value = ps;
        return true;        // always return true for this, since we treat all errors as "zero".
    }

    virtual const char* Name() const override {
        return "MergeTestOperator";
    }
};


void getColumnFamilyHandles(DB *db, map<string, string> &colname, vector<ColumnFamilyHandle*> *handles, bool all=false) {
    string name("/data0/home/guangtong/pgt_test/rocksdb_dir");
    std::vector<ColumnFamilyDescriptor> column_families;
    std::vector<std::string> column_name;
    db->ListColumnFamilies(DBOptions(), name, &column_name);    
    std::vector<std::string>::iterator it;
    for (it = column_name.begin(); it != column_name.end(); it++) {
        if (all || (colname.find(*it) != colname.end())) {
            ColumnFamilyDescriptor cdc = ColumnFamilyDescriptor(*it, ColumnFamilyOptions());
            column_families.push_back(cdc);
        }
    }

    rocksdb::DB::OpenForReadOnly(DBOptions(), name, column_families, handles, &db);
}


int main() {
    rocksdb::DB* db; 
    rocksdb::DBWithTTL* ttldb;
    rocksdb::Options options;
    options.create_if_missing = true;
    options.write_buffer_size = 1*1024;
    options.merge_operator.reset(new MergeTestOperator);  // 数据库一旦用merge操作选项打开，并且有merge操作，那么之后这个数据库再次打开的时候，必须也要使用merge option打开，不然的话，目前已知seek的时候会报错
    //rocksdb::Status status = rocksdb::DB::Open(options, "/data0/home/guangtong/pgt_test/rocksdb_dir", &db);    
    rocksdb::Status status;
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
            if (p->i == 101) {  // 建立数据库
                cout << "hello" << endl;
                status = rocksdb::DB::Open(options, "/data0/home/guangtong/pgt_test/rocksdb_dir", &db);    
                cout << "world" << endl;
                if (!status.ok()) {
                    cout << "open db failed " << status.ToString() << endl;
                }
                else {
                    cout << "open db success " << endl;
                }
            }
            /*
             * ttl的db，在不进行compact之前，即时一个key过期了，仍然也是能够去得到的
             * 只有在进行compact的时候，才会把过期的key给删除了
             * 删除的这个功能是rocksdb自带的，不用自己再写compact filter了
             * ttl其实就是在value的结尾加上了一个时间字段
            */
            else if (p->i == 102) {    // open with ttl
                status = DBWithTTL::Open(options, "/data0/home/guangtong/pgt_test/rocksdb_dir", &ttldb, 20);
                if (!status.ok()) {
                    cout << "open db failed " << status.ToString() << endl;
                }
                else {
                    cout << "open db with ttl success " << endl;
                    db = ttldb;
                }
            }
            else if (p->i == 0) {  // 写操作
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
                // RestoreDBFromBackup这个函数还原的数据放在backdir里面，里面的文件格式和rocksdb原始的目录下面的文件是一样的，而不时backup目录下的文件那样子的
                // backup id要从backup的目录里面得到，private目录下对应的数字就是id，或者通过getbackupinfo得到
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
                it->Seek(p->kbuf);   // seek的时候，如果要seek的key不存在，则就seek到这个key之后第一个存在的key的位置
                if (!it->Valid()) {
                    cout << "seek to " << p->kbuf << " failed " << it->status().ToString() << endl;
                }
                cout << it->key().ToString() << " : " << it->value().ToString() << endl;
                delete it;
            }
            else if (p->i == 7) {    // 新建columnfamily
                ColumnFamilyHandle* cf;
                char *cfname = p->vbuf;
                cout << "before create columnfamily " << cfname << endl;
                status = db->CreateColumnFamily(ColumnFamilyOptions(), cfname, &cf);
                if (!status.ok()) {
                    cout << "create columnfamily " << cfname << "error\n";
                }
                cout << "create columnfamily " << cfname << "success\n";
                std::vector<std::string> column_families;
                string name("/data0/home/guangtong/pgt_test/rocksdb_dir");
                db->ListColumnFamilies(DBOptions(), name, &column_families);
                cout << "list columnfamily \n";
                std::vector<std::string>::iterator it;
                for (it = column_families.begin(); it != column_families.end(); it++) {
                    cout << "cf is " << *it << endl;
                }

            }
            else if (p->i == 8) {    // 删除columnfamily
                string name("/data0/home/guangtong/pgt_test/rocksdb_dir");
                std::vector<ColumnFamilyDescriptor> column_families;
                std::vector<std::string> column_name;
                db->ListColumnFamilies(DBOptions(), name, &column_name);
                std::vector<std::string>::iterator it;
                for (it = column_name.begin(); it != column_name.end(); it++) {
                    ColumnFamilyDescriptor cdc = ColumnFamilyDescriptor(*it, ColumnFamilyOptions());
                    column_families.push_back(cdc);
                    //column_families.push_back(ColumnFamilyDescriptor(*it, ColumnFamilyOptions()));
                }
                
                string delcol(p->vbuf);
                bool mark = false;
                std::vector<ColumnFamilyHandle*> handles;
                ColumnFamilyHandle* delhandles = NULL;
                cout << "get all column family handle" << endl;
                rocksdb::DB::OpenForReadOnly(DBOptions(), name, column_families, &handles, &db);
                std::vector<ColumnFamilyHandle*>::iterator itt;
                for (itt = handles.begin(); itt != handles.end(); itt++) {
                    cout << "cf is " << (*itt)->GetName() << endl;
                    if ((*itt)->GetName().compare(delcol) == 0) {
                        delhandles = *itt;
                        break;
                    }
                }
                if (delhandles) {
                    cout << "delete column family " << delhandles->GetName() << endl;
                    db->DropColumnFamily(delhandles);
                    delete delhandles;
                }

            }
            else if (p->i == 9) {    // flush
                string s(p->vbuf);
                stringstream ss(s);
                map<string, string> colname;
                while(ss.good()) {
                    string substr;
                    getline(ss, substr, ',');
                    colname[substr] = "";
                }
                vector<ColumnFamilyHandle*> handles;
                bool all = false;
                if (s.compare("ALL") == 0)
                    all = true;
                getColumnFamilyHandles(db, colname, &handles, all);                
                vector<ColumnFamilyHandle*>::iterator it;
                for (it = handles.begin(); it != handles.end(); it++) {
                    cout << "flush column " << (*it)->GetName() << endl;
                    db->Flush(FlushOptions(), (*it));
                }


            }
            else if (p->i == 100) {    // 退出程序
                std::vector<ColumnFamilyHandle*> handles;
                const std::vector<ColumnFamilyDescriptor> column_families;
                string name("/data0/home/guangtong/pgt_test/rocksdb_dir");
                rocksdb::DB::OpenForReadOnly(DBOptions(), name, column_families, &handles, &db);
                std::vector<ColumnFamilyHandle*>::iterator it;
                for (it = handles.begin(); it != handles.end(); it++) {
                    db->DropColumnFamily(*it);
                    delete *it;
                }
                delete db;

            }
            else if (p->i == 10) {    // 单独删除
                /* 同一个key可以删除多次，都是不会报错的，只不过在删除了之后，get的时候，会报错key不存在
                 * 估计就是删除的时候只是在key上做了个标记，表示被删除了，但是key在库里还是存在的，所以重复删除就是重复做标记，
                 * 但是get的时候，因为读取到删除标记，所以就知道被删除了，所以就报错不存在了
                 * 如果是一个根本就不存在的key(而不是打上删除标记的key)，删除操作也是不会报错的
                */
                status = db->Delete(rocksdb::WriteOptions(), p->kbuf);
                if (!status.ok()) {
                    if (status.IsNotFound() == true) {
                        cout << "delete key failed " << status.ToString() << " not found" << endl;
                    }
                }
                else {
                    cout << "delete key " << p->kbuf << " success " << endl;
                }
            }
            else if (p->i == 12) {    // merge操作
                rocksdb::WriteOptions options;
                auto s = db->Merge(options, p->kbuf, p->vbuf);
                if (s.ok()) {
                    cout << "merge key " << p->kbuf << " ok " << endl;
                } else {
                    std::cerr << s.ToString() << std::endl;
                }
                string vv;
                status = db->Get(rocksdb::ReadOptions(), p->kbuf, &vv);
                if (!status.ok()) {
                    cout << "get merge key " << p->kbuf << " failed " << endl;
                }
                else {
                    cout << "get merge key " << p->kbuf << " value " << vv << " success " << endl;
                }

            }
            else if (p->i == 13) {    // open with checkpoint
                Checkpoint *checkpoint_ptr;
                const string checkpoint_dir = "/data0/home/guangtong/pgt_test/rocksdb_checkpoint_dir";
                status = Checkpoint::Create(db, &checkpoint_ptr);
                if (!status.ok()) {
                    cout << "create checkpoint ptr err " << status.ToString() << endl;
                }
                else {
                    status = checkpoint_ptr->CreateCheckpoint(checkpoint_dir);
                    if (!status.ok()) {
                        cout << "create checkpoint err " << status.ToString() << endl;
                    }
                }
            }
        }
    }
    free(p);
    close(sockfd);
    return 0;
}
