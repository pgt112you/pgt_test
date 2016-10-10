#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>



int main() {
  char buf[500];
  memset(buf, '\0', sizeof(char)*500);
  buf[0] = 'a';
  buf[1] = 'b';
  buf[2] = 'c';
  buf[3] = '\n';
  //buf[4] = 'd';
  //std::string data(buf);
  std::string data;
  data = buf;
  std::cout << "data is " << data << std::endl;
  if (*(data.end()-2) == 'c') {
    printf("match\n");
  }
  std::cout << *(data.end()-1) << std::endl;
}


