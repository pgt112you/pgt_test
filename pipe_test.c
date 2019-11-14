#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 * /proc/pid/fd里面对应的是pipe:[152466430]这种的,而且子进程和父进程都是,并且两个的中括号里面的数字一样的
*/

int main() {
    int pipefd[2];
    pid_t cpid;
    char *buf = malloc(64);
    bzero(buf, 64);

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {    /* Child reads from pipe */
        close(pipefd[1]);          /* Close unused write end */
    } else {            /* Parent writes argv[1] to pipe */
        close(pipefd[0]);          /* Close unused read end */
    }
    int i = 0;
    int res;
    while(1) {
        if (cpid == 0) {
            bzero(buf, 64);
            res = read(pipefd[0], buf, 64);
            if (res < 0) {
                 perror("salve read Error"); 
            }
            printf("child read: %s\n", buf);
        } else {
            i++;
            bzero(buf, 64);
            sprintf(buf, "hello world %d", i);
            res = write(pipefd[1], buf, strlen(buf));
            if (res < 0) {
                 perror("Master write Error"); 
            }
            printf("master write: %s\n", buf);
            sleep(2);
        }
    }
    if (cpid != 0) {
        wait(NULL);
    }
}
