#include "pgt_test.h"
#include <iostream>

using namespace std;


int main() {
    int *p = new int{5};
    cout <<*p <<endl;
    delete p;
    int *p1 = new int[5];
    p1[0] = 3;
    p1[1] = 4;
    p1[2] = 5;
    cout << *p1 << "\t" << *(p1+1) << "\t" << *(p1+2) << endl;
    delete [] p1;

    return 0;
}
