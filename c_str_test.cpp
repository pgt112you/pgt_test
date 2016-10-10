#include <iostream>
#include <string>

using namespace std;

int main() {
  const char *s;
  if (1 == 1) 
    string ss = "aaaa";
    s = (ss + "cba").c_str();
  printf("s is %s\n", s);
  return 0;
}
