#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>           /* Definition of AT_* constants */
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


int main() {
    int res = mkfifo("./test.fifo", S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
    if ((res < 0) && (errno != EEXIST)) {
        perror("Error"); 
    }
    //int wfd = open("./test.fifo", O_APPEND, O_WRONLY);
    int wfd = open("./test.fifo", O_WRONLY);
    if (wfd == -1) {
        perror("Open error"); 
    }
    char *content = (char *)malloc(32);
    int i = 0;
    while(1) {
        sleep(2);
        i++;
        bzero(content, 32);
        sprintf(content, "%s %d\n", "hello world", i);
        res = write(wfd, content, strlen(content));
        if (res < 0) {
            perror("Error"); 
        }
        printf("write content %s\n", content);
    }

}
