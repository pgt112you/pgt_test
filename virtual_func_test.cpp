#include <iostream>


using namespace std;


class F {
  public:
    int a;
    F(int i):a(i){};
    virtual void show();
    //void show();
    virtual ~F();
};

void F::show() {
    cout << "in father class " << a << endl;
}

F::~F() {
    cout << "in father destructor" << endl;
}

class C:public F {
  public:
    int a;
    C(int i):F(i-1),a(i){};
    void show();
    ~C() {cout << "in child destructor" << endl;};
};

void C::show() {
    //F::show();
    cout << "in child class " << a << endl;
}



int main() {
    F f(2);
    F *pc = new C(6);
    f.show();
    pc->show();
    C *pcc = new C(9);
    pcc->show();
    cout << "========================" << endl;
    delete pc;
    cout << "========================" << endl;
    delete pcc;
    cout << "========================" << endl;
    return 0;
}

