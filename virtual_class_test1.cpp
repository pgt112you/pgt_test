#include "pgt_test.h"
#include <iostream>


using namespace std;


class P {
 public:
   P(int i, int j):p1(i),p2(j) {
     cout << "in P construct\n";      
   }
   int p1, p2;
   ~P() {
     cout << "in P desconstruct" << endl;
   }

   virtual void show() {
     cout << "P element is " << p1 << "\t" << p2 << endl;
   }
};

class C:public P {
 public:
  C(int i, int j):P((i-1),(j-1)),c1(i),c2(j) {
    cout << "in C construct\n";      
  }
  ~C() {
    cout << "in C desconstruct" << endl;
  }
  int c1, c2;

  void show() {
    cout << "C element is " << c1 << "\t" << c2 << endl;
  }
};


int main() {
  P *pp = new C(5,6);
  pp->show();
  delete pp;
  return 0;
}
