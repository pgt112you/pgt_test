#include "pgt_test.h"
#include <memory>
#include <iostream>
#include <unistd.h>

using namespace std;

void func() {    // 这个最后两个输出都是35
    //shared_ptr<int> pi(new int{5});    // 这一行和下面一行都可以
    shared_ptr<int> pi(new int(5));
    *pi = 6;
    shared_ptr<int> pi1 = make_shared<int> (35);
    pi = pi1;
    
    cout << *pi << endl;
    cout << *pi1 << endl;
}

void func1() {   // 这个会挂掉
    auto_ptr<int> pi(new int(5));
    *pi = 6;
    auto_ptr<int> pi1 = pi;
    pi = pi1;
    
    cout << *pi << endl;
    cout << *pi1 << endl;
}

void func2() {
    unique_ptr<int> pi(new int(5));
    unique_ptr<int> pi1(new int(15));
    pi.reset(pi1.release());
    pi1.reset(new int{25});
    cout << *pi << endl;
    cout << *pi1 << endl;
}

int main() {
    func();
    sleep(1);
    //func1();
    sleep(1);
    func2();
}
