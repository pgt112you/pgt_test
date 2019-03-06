#include "pgt_test.h"
#include <iostream>


using namespace std;

template <typename T> 
T add(T a, T b) {
    return a+b;
}

//template <type T>  // 函数模板只能写typename，不能写type，类模板可以写class
//T sub(T a, T b) {
//    return a - b;
//}

///////////////////////////////////////////////////////////////////////////////////

template <typename C>
class Hello {
 public:
  Hello(C c1, C c2):i(c1),j(c2){};
  C i;
  C j;
  //C add() {return i+j;}
  C add();
};
template <typename C>  // 在模板类的外部定义模板类函数，需要在重新写一遍template <typename C>
C Hello<C>::add() {    // 在类模板外定义，一定要在类后面加上<C>
   return i+j;
};

template <class C>
//template <typename C>
class World {
 public:
    C sub(C c1, C c2) {
        return c1 - c2;
    }
    C add(C c1, C c2);
};
template <class C>
C World<C>::add(C c1, C c2) {
    return c1 + c2;
}

int main() {
    int i(5);
    int j = 6;
    int k = add(i, j);
    cout << k << endl;
    Hello<int> h(i, j);
    cout << h.add() << endl;

    //int k1 = sub(j, i);
    //cout << k1 << endl;
    World<int> w;
    cout << w.sub(i, j) << endl;
    cout << w.add(i, j) << endl;
    return 0;
}
