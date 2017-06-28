#include "pgt_test.h"
#include <iostream>

using namespace std;

class Base {
  public:
    static void Create(int i, Base **pb);
    virtual void Check(int);
};


class BaseImpl:public Base {
  public:
    explicit BaseImpl(int i) :i_(i) {}
    using Base::Check;
    virtual void Check(int) override;

  private:
    int i_;
};

void Base::Create(int i, Base **pb) {
    *pb = new BaseImpl(i);
}

void Base::Check(int j) {
    cout << "base check, it is not run in normal model" << endl;
}

void BaseImpl::Check(int j) {
    cout << "in baseimpl check j is " << j << endl;
}


int main() {
    Base *pb;
    Base::Create(8, &pb);
    pb->Check(7);

}
