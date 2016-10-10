/*
 * 测试结构体的内存分配是不是和定义的顺序是一致的
 * 比如：
 * struct test {
 *   int i;
 *   bool a;
 *   bool b;
 * };
 * 的内存分配是不是先分配8个bit给i，然后分配一个bit给a，再分配一个bit给b
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> 
#include <string.h>


int main() {
  struct test {
    bool b1:1;
    unsigned int i1:8;
    bool b2:1;
    unsigned int i2:6;
  };
  //char *str = (char *)malloc(sizeof(char)*2);
  //*str = 'a';
  //*(str+1) = '0';
  char str[2] = {'a', '0'};
  // p_test is aa  converted into binary is 0 11000010 0 110000
  struct test *p_test = (struct test*)(void *)str;
  if ((p_test->b1 << 7) == 128)
    printf("b1 is ture\n");
  else
    printf("b1 is false\n");

  if ((p_test->b2 << 7) == 128)
    printf("b2 is ture\n");
  else
    printf("b2 is false\n");

  printf("p_test is %s\n", p_test);
  printf("i1 is %u\n", p_test->i1);
  printf("i2 is %u\n", (p_test->i2)<<2);
  //free(str);
}
