#include<stdlib.h>
#include<stdio.h>


class AA {
 public:
  int a;
};

int main() {
  AA *p_a = new AA;
  delete p_a;
  printf("aaaaa\n");

}
