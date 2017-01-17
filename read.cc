#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main() {
    FILE *F = fopen("test11.dat1484130357830" ,"r");
    int i;
    bzero(&i, sizeof(i));
    fread(&i, 8, 1, F);
    printf("i is %d\n", i);
    return 0;
}
