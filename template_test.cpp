#include <iostream>


using namespace std;


template<typename T>
T show(T i) {
    cout << "input " << i << endl;
    return i+1;
}

template<typename T>
class C {
  public:
    T a;
    C(T i):a(i) {};
    T show(T i) {
        cout << "input " << i << endl;
        return i+1;
    }
};


int main() {
    int i, j;
    i = 5;
    j = show(i);
    cout << "j is " << j << endl;
    C<int> c(11);
    int k = c.show(8);
    cout << "k is " << k << endl;
    cout << "flat show " << show(2.53) << endl;
    return 0;
}
