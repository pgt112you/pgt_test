#include "pgt_test.h"
#include <iostream>

using namespace std;

class ObjA {
  public:
    int i;
    ObjA(int code):i(code){}
    static ObjA i5() {
        return ObjA(5);
    }
    static ObjA i6() {
        return ObjA(6);
    }
    static ObjA i7() {
        return ObjA(7);
    }
};

int main() {
    ObjA a5 = ObjA::i5();
    ObjA a6 = ObjA::i6();
    ObjA a7 = ObjA::i7();
    cout << a5.i << a6.i << a7.i << endl;

}
