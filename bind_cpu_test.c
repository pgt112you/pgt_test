#define _GNU_SOURCE 
#include <sched.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>


int main() {
    cpu_set_t   mask;
    CPU_ZERO(&mask);
    CPU_SET(3, &mask);
    if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1) {
        printf("bind cpu error %s", strerror(errno));
        return 0;
    }
    int i = 0;
    while(1) {
        i++;
        if (i > 10000000) {
            i = 0;
        }
    }
    return 0;

}
