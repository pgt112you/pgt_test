/* 可变参数测试 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


void func(char *first, int second, ...) {
    va_list ap;
    int i;
    char *s;
    va_start(ap, second);
    i = va_arg(ap, int);
    s = va_arg(ap, char*);
    printf("i is %d\n", i);
    printf("s is %s\n", s);
    va_end(ap);
    return;

}

int main() {
    func("35", 25, 11,  "hello world", 11);
    return -1;

}
