#include "pgt_test.h"


extern char **environ;

int main() {
    char **ep;
    for (ep = environ; *ep != NULL; *ep++) {
        puts(*ep);
        printf(">>> %s\n", *ep);
    }
    return 0;
}
