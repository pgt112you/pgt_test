#include "pgt_test.h"
#include <iostream>
#include <bitset> 


using namespace std;

int main() {
    bitset<32> foo;
    foo.set(30, 1);
    unsigned int b = foo.to_ulong();
    cout << "b is " << b << endl;
    unsigned int i = b | 32;
    cout << "i is " << i << endl;
    
    return 0;
}
