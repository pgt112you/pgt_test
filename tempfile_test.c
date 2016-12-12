#include "pgt_test.h"
#include <stdlib.h>



int main() {
    int fd;
    char template[] = "/home/guangtong/pgt_test/abcdXXXXXX";
    //char *template = "/home/guangtong/pgt_test/abcdXXXXXX";  // 这里不能为这种形式，这样的话会coredump的，因为mkstemp函数会修改XXXXXX这几个值，字符串常量是不允许修改的
    fd = mkstemp(template);
    close(fd);


}
