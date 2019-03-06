#include "pgt_test.h"
#include <iostream>

using namespace std;

class C {
 public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  int i;
  C(int ii):i(ii){
    cout << "in c instruct func count is " << endl;
    throw 11; // 这个异常会触发operator delete函数
  };
  ~C(){cout << "in c des instruct func" << endl;};
};

void *C::operator new(std::size_t count) {
  cout << "in c operate new count is " << count << endl;
  //throw 16;  // 这个异常不会触发operator delete函数
  return ::operator new(count);
}

void C::operator delete(void *ptr) {
  cout << "in c operate delete" << endl;
  ::operator delete(ptr); 
}

int main() {
  try {
    C *pc = new C(2);
    cout << pc->i << endl;
    delete pc;
  } catch(int ie) {
    cout << "err number is " << ie << endl;
  }
  cout << "==================================================\n";
  try {
    C pc1(3);
    cout << pc1.i << endl;
  } catch(int ie) {
    cout << "err22 number is " << ie << endl;
  }
  return 0;
}
