/*
 * 测试一下结构体里的一个指针指向一块地址
 * new出一个结构体对象之后，先delete调指针，然后再delete调结构体对象
 * 看看程序是否报错
 * 结论：不报错。没问题
*/
#include "pgt_test.h"



struct t_stru {
    int i;
    char *s;
    char c;
};


int main() {
    struct t_stru *p_t = new struct t_stru;
    p_t->s = new char[15];
    p_t->i = 6;
    p_t->c = 'a';
    strcpy(p_t->s, "hello world");
    delete p_t->s;
    delete p_t;
    return 0;
}
