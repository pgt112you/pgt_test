#include <stdlib.h>
#include <stdio.h>
#include <vector>

/*
  测试vector中的数据是深copy还是浅copy
*/

using namespace std;

int main() {
  int il[6] = {1,2,3,4,5,6};
  int i;
  vector<int> ivec;
  for (i=0; i<6; i++) {
    ivec.push_back(il[i]);
  }
  il[2] = 22;
  printf("il 2 is %d\n", il[2]);
  printf("ivec 2 is %d\n", ivec[2]);
}
