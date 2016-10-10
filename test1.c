#include <stdlib.h>
#include <stdio.h>

int a = 3;

int func() {
  printf("in func a is %d\n", a);
}


int main() {
  int a = 5;
  printf("in main a is %d\n", a);
  func();

}
