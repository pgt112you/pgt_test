#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int main() {
  struct stat s;
  stat("/usr/local/sinawatch_agent/data/temp.data", &s);
  printf("time of last access is %ld\n", s.st_atime);
  printf("time of last modification is %ld\n", s.st_mtime);
  printf("time of last status change is %ld\n", s.st_ctime);

  return 0;
}
