#include <iostream>

using namespace std;

class P {
  public:
    int i;
    P(int x):i(x){};
    P();
    virtual int show();
};

int P::show() {
    return i+1;
}

P::P() {
    i = 6;
}

int main() {
    P p(5);
    cout << "p is " << p.show() << endl;
    P p1;
    cout << "p1 is " << p1.show() << endl;
    return 0;
}
