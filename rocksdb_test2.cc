#include <iostream>
#include "rocksdb/db.h"


using namespace std;


int main() {
    rocksdb::DB* db;
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::Status status = rocksdb::DB::Open(options, "/data0/home/guangtong/pgt_test/rocksdb_dir_test11", &db);
    status = db->Put(rocksdb::WriteOptions(), "keke", "123456");
    status = db->Put(rocksdb::WriteOptions(), "keke11", "123456");
    status = db->Put(rocksdb::WriteOptions(), "hehe11", "123456");
    rocksdb::ReadOptions ropt;
    ropt.snapshot = db->GetSnapshot();
    status = db->Put(rocksdb::WriteOptions(), "gao", "123456");
    status = db->Put(rocksdb::WriteOptions(), "lu", "123456");
    status = db->Put(rocksdb::WriteOptions(), "laolujie", "123456");
    status = db->Put(rocksdb::WriteOptions(), "hehe11", "654321");
    rocksdb::Iterator* it = db->NewIterator(ropt);
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        cout << it->key().ToString() << ": " << it->value().ToString() << endl; 
    }

    delete it;
    db->ReleaseSnapshot(ropt.snapshot);

    rocksdb::ReadOptions ropt1;    // 这个readoptions只能重新定义，不能服用ropt，不然还是快照的内容，即使是删了选项中的快照了
    rocksdb::Iterator* it1 = db->NewIterator(ropt1);
    cout << "==================================================\n";
    for (it1->SeekToFirst(); it1->Valid(); it1->Next()) {
        cout << it1->key().ToString() << ": " << it1->value().ToString() << endl; 
    }

    delete it1;
    delete db;


    return 0;
}
