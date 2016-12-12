#include "pgt_test.h"
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>


void finish_child(int sig) {
    printf("finish child process\n");
    sleep(200);
    if (wait() < 0) {
        printf("wait child process failed\n");
    }
    return;
}


void handler(int sig) {
     printf("func handler process %d signal %d\n", getpid(), sig);    
     return;
}


void handler11(int sig) {
     printf("func handler11 process %d signal %d\n", getpid(), sig);    
     return;
}


int main() {
    signal(SIGCHLD, finish_child);
    struct sigaction sa;
    sigfillset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGUSR1, &sa, NULL) < 0) {
        printf("sigaction error\n");
        return 0;
    }
    printf("parent pid is %d\n", getpid());
    int pid = fork();
    if (pid == 0) {
        struct sigaction sa1;
        sigfillset(&sa1.sa_mask);
        sa1.sa_flags = 0;
        sa1.sa_handler = handler11;
        if (sigaction(SIGUSR1, &sa1, NULL) < 0) {
            printf("sigaction error\n");
            return 0;
        }
    }
    pause();

    return 0;
}
