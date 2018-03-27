#include<iostream>

using namespace std;

int& func() {
   int i = 5;
   return i;
}


int main() {
    int j = func();
    cout << j << endl;
    return 0;
}
