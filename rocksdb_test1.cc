#include <iostream>
#include "rocksdb/db.h"


using namespace std;


int main() {
    rocksdb::DB* db;
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::Status status = rocksdb::DB::Open(options, "/data0/home/guangtong/pgt_test/rocksdb_dir_test", &db);
    rocksdb::Iterator* it = db->NewIterator(rocksdb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        cout << it->key().ToString() << ": " << it->value().ToString() << endl;    
    }
    cout << "hello world" << endl;
    assert(it->status().ok());
    cout << "hello world 11" << endl;
    delete db;


    return 0;
}
