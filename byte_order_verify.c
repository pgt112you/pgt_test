#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>


int main() {
  union {
    short s;
    char c[sizeof(short)];
  } un;

  un.s = 0x0102;
  printf("byte order: ");
  if (sizeof(short) == 2) {
    if (un.c[0] == 1 && un.c[1] == 2) 
      printf("big-endian\n");
    else if (un.c[0] == 2 && un.c[1] == 1)
      printf("little-endian\n");
    else
      printf("unknown\n");
  }
  else
    printf("sizeof(short) = %d\n", sizeof(short));

  /* 将变量转换成网络字节序（大端序），这样看看再打印是否正确 */
  union {
    short s;
    char c[sizeof(short)];
  } un1;
  un1.s = htons(0x0102);
  printf("0x0102 is %x\n", 0x0102);
  printf("un1.s is %x\n", un1.s);
  printf("un1 1 is %d\n", un1.c[0]);
  printf("un1 2 is %d\n", un1.c[1]);

  return 0;

}
