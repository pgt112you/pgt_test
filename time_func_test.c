#include <time.h>
#include <sys/time.h>


int main() {
  time_t t = time(NULL);
  struct tm *tt = localtime(&t);
  printf("day is %d\n", tt->tm_mday);
  printf("hour is %d\n", tt->tm_hour);
  printf("min is %d\n", tt->tm_min);
  printf("sec is %d\n", tt->tm_sec);
}
