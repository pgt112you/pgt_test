#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>           /* Definition of AT_* constants */
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


/*
 * /proc/pid/fd里面对应的是fifo的文件路径
*/

int main() {
    int res = mkfifo("./test.fifo", S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
    if ((res < 0) && (errno != EEXIST)) {
        perror("Error"); 
    }
    int rfd = open("./test.fifo", O_RDONLY);
    char *content = (char *)malloc(32);
    while(1) {
        sleep(2);
        bzero(content, 32);
        int i = read(rfd, content, 32);
        printf("%s %d\n", content, i);
    }

}
