#include "pgt_test.h"

int main() {
    printf("page size is %d\n", getpagesize());
    printf("page size11 is %d\n", sysconf(_SC_PAGESIZE));
    return 0;
}
