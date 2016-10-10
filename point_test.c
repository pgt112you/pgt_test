#include "pgt_test.h"

struct tt {
    int i;
    char c;
};


int main () {
    struct tt *p_t;
    printf("*p_t is %d\n", sizeof(*p_t));
    printf("struct tt is %d\n", sizeof(struct tt));
}
