/*
 * 默认情况下goto的各个模块都会走到的
 * 即使没有直接goto到done的语句，done模块也会走到的
 * test模块也是一样的，顺序执行到这里，就会执行test里面的内容
*/

#include "pgt_test.h"


int main() {
    int i = 4;
    printf("aaaa\n");
    if (i > 5) {
        goto test;
    }
    printf("bbbb\n");

test:
    printf("in test\n");
done:
    printf("in done\n");
    return;
}
