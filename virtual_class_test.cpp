#include "pgt_test/pgt_test.h"


class A {
  public:
    int i;
    A(int i, int j):i(i),j(j){printf("in A structor\n");}
    virtual int func(int);
    //virtual ~A(){printf("in A destructor\n");}
    virtual ~A(){printf("in A destructor\n");}
  private:
    int j;
};

int A::func(int fi) {
    printf("in A func fi is %d, j is %d\n", fi, j);
}


class B:public A {
  public:
    int k;
    B(int i, int j):A(i,j), k(i){printf("in B structor\n");}
    ~B(){printf("in B destructor\n");}

};

int main() {
    B *pb = new B(2, 3);
    pb->func(6);
    delete pb;
    //A a1();     // 已经自己定义了构造函数，系统就不会再给构造默认构造函数了
    //a1.func(7);
    return 0;
}
