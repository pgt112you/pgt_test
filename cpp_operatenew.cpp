#include <iostream>


using namespace std;

class Test {
  public:
    int i;
    Test(int ii);
    Test();
    ~Test();
    static void *operator new(size_t t);
    static void operator delete(void *ptr);
};


Test::Test(int ii) {
    cout << "in constructor func with paramater" << endl;
    if (ii > 0) {
        i = ii;
        return;
    }
    throw 5;
}

Test::Test() {
    cout << "in constructor func" << endl;
    throw 6;
}

Test::~Test() {
    cout << "in destructor func" << endl;
}

void *Test::operator new(size_t t) {
    cout << "in oeprator new " << endl;
    void *ptr = ::operator new(t);
    return ptr;
}

void Test::operator delete(void *ptr) {
    cout << "in oeprator delete " << endl;
    ::operator delete(ptr);
}


int main() {
    try {
        //Test *pt = new Test;
        Test *pt = new Test(-4);
        pt->i = 8;
        cout << pt->i << endl;
    } catch (int ii){
        cout << "in catch " << ii << endl;
    }
    // Test *pt1 = new Test(-8);  // 如果不抛异常会aborted
    Test *pt1 = new Test(8);  
    delete pt1;
    return 0;
}
