#include <iostream>
#include "rocksdb/db.h"
#include "rocksdb/utilities/backupable_db.h"
#include "rocksdb/slice.h"


using namespace std;
using namespace rocksdb;


int main() {
    rocksdb::DB* db;
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::Status status = rocksdb::DB::Open(options, "/data0/home/guangtong/pgt_test/rocksdb_dir", &db);
    string value;
    string key1 = "hello";
    string key2("world");
    rocksdb::Status s = db->Put(rocksdb::WriteOptions(), key1, "honda");
    s = db->Get(rocksdb::ReadOptions(), key1, &value);
    cout << "get status is " << s.ToString() << endl;
    s = db->Put(rocksdb::WriteOptions(), key2, value);
    s = db->Put(rocksdb::WriteOptions(), "Toyota", "COROLLA");
    s = db->Put(rocksdb::WriteOptions(), "Honda", "FIT");
    s = db->Put(rocksdb::WriteOptions(), "Honda", "CIVIC");
    s = db->Put(rocksdb::WriteOptions(), "Volkswagen", "Golf");
    s = db->Put(rocksdb::WriteOptions(), "QINGDAO", "SHANDONG");
    s = db->Put(rocksdb::WriteOptions(), "YANTAI", "SHANDONG");
    s = db->Put(rocksdb::WriteOptions(), "SHANDONG-jinan", "shenghui");
    s = db->Put(rocksdb::WriteOptions(), "SHANDONG-linyi", "danongcun");
    s = db->Put(rocksdb::WriteOptions(), "SHANDONG-qufu", "pianzi");
    s = db->Put(rocksdb::WriteOptions(), "SHANDONGSHENG", "province");


    rocksdb::Iterator* it = db->NewIterator(rocksdb::ReadOptions());
    Slice sl("SHANDONG");
    it->Seek(sl);
    cout << "hello world" << endl;
    for (; it->Valid(); it->Next()) {
        cout << it->key().ToString() << ": " << it->value().ToString() << endl;    
    }
    cout << "hello world 11" << endl;
    /*
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        cout << it->key().ToString() << ": " << it->value().ToString() << endl;    
    }
    cout << "hello world" << endl;
    assert(it->status().ok());
    cout << "hello world 11" << endl;
    */
    
    delete it;
    delete db;

    return 0;
}
