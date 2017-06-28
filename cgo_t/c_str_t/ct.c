#include "ct.h"
#include <string.h>


char *print(int i) {
    if (i > 0) {
        char *s = (char *)malloc(11);
        strcpy(s, "hello world");
        return s;
        //return "hello world";
    } else {
        return NULL;
    }

}
