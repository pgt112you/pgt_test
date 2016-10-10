/*
 * 写二进制的数据到文件里
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>


int main() {
  struct test {
    char *s;
    int i;
    char c;
    union {
      int ui;
      char uc;
    } u;
  };
  struct test t;
  t.s = "hello world";
  t.i = 15;
  t.c = 't';
  t.u.ui = 25;
  int fd = open("test.data", O_WRONLY|O_CREAT);
  write(fd, &t, sizeof(t));
  close(fd);
}
