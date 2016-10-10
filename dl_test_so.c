#include <stdlib.h>
#include <stdio.h>


char *getdata(char *name) {
  FILE *pf = popen(name ,"r");
  char *p = (char *)malloc(sizeof(char)*50);
  fread(p, sizeof(char), 50, pf);
  pclose(pf);
  return p;
}

