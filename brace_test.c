#include "pgt_test.h"

int main() {
    int i = 5;
    printf("before brace i is %d\n", i);
    {
        i++;
        printf("in brace i is %d\n", i);
    }
    printf("after brace i is %d\n", i);
    return 0;
}
