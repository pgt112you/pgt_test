#include "pgt_test.h"



typedef struct {
    int i;
    char c;
    void *(*func)(int);
    int j;
    int (*func1)();
    char c1;
} strss;


int main() {
    strss s;
    int i = 5/0;
    return 0;

}
