#include "pgt_test.h"
#include <unistd.h>



int main() {
    printf("_PC_NAME_MAX is %d\n", pathconf("/home/guangtong/pgt_test", _PC_NAME_MAX));
    printf("_PC_PATH_MAX is %d\n", pathconf("/home/guangtong/pgt_test/", _PC_PATH_MAX));
    printf("_PC_PIPE_BUF is %d\n", pathconf("/home/guangtong/pgt_test", _PC_PIPE_BUF));
    return 0;
}
