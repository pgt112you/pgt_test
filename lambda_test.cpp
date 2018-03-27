#include "pgt_test.h"
#include <iostream>


using namespace std;


int main() {
    int a = 5;
    auto f = [a](int &i, int &j){i++; return j+i+a;};
    int b = 8;
    int c = 9;
    cout << f(b, c) << endl;
    printf("%d\t%d\t%d\n", a,b,c);
    return 0;


}
