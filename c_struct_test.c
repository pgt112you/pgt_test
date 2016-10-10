/*
 * 看看能不能在结构体声明的时候设置默认值
 * 结论：不能
*/

#include <stdlib.h>
#include <stdio.h>


struct tt {
    char c;
    //int i = 5;
    int i;
};



int main() {
    struct tt t;
    t.c = 'a';
    printf("t.c is %c t.i is %d\n", t.c, t.i);
    return 0;
}
