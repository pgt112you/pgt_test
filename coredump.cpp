#include <iostream>

using namespace std;


class F {
  public:
    int i;
    virtual void show();
    void print();
    int j;
    virtual void show11();
    virtual ~F() {};
};


void F::show() {
    cout << "hello" << endl;
}

void F::show11() {
    cout << "hello11" << endl;
}

void F::print() {
    cout << "world" <<  endl;
}


class C:public F {
  public:
    int a;
    void show();
    int b;
    virtual void exhibition() {};
    virtual ~C() {};
};

void C::show() {
    cout << "HELLO WORLD" << endl;
}

class O {
  public:
    int o1;
    void show();
    int func() {return 1;}
    char c;
  private:
    int o2;
};


int main() {
    F f1();
    F *pf = new F();
    F *pf1 = new C();
    C *pc = new C();
    O *po = new O;
    int c = 5/0;
    return 0;
}
