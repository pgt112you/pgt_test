#include "pgt_test.h"



int main() {
    int i = 165510;
    switch (i) {
      case 0:
          printf("case 0\n");
          break;
      case 126:
          printf("case 126\n");
          break;
      case 244:
          printf("case 244\n");
          break;
      case 510:
          printf("case 510\n");
          break;
      case 65510:
          printf("case 65510\n");
          break;
      case 165510:
          printf("case 165510\n");
          break;
      defalut:
          printf("default case %d\n", i);
          break;
    }

    return 0;
}
