#include "pgt_test.h"
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define MAX_EVENTS 10


void deal_fd(int ffd, int fd, int wd) {
    const struct inotify_event *event;
    char buf[4096];
    int len = read(fd, buf, 4096);
    if (len < 0) {
        return;
    }
    char *ptr;
    for (ptr = buf; ptr < buf + len; ptr += sizeof(struct inotify_event) + event->len) {
        event = (const struct inotify_event *) ptr;
        if (!(event->mask & IN_MODIFY)) {
            continue;      
        }
        if (event->wd == wd)
            break;
    }

    char *content[4096];
    while(1) {
        bzero(content, 4096);
        printf("----------------------------------------------------------------\n");
        len = read(ffd, content, 4096);
        printf("================================================================\n");
        printf("%s", content);
        //if (len < 4096) {
        if (len <= 0) {
            break;
        }
    }

    return;
}




int main() {
    int fd;
    int wd;
    fd = inotify_init1(IN_NONBLOCK);
    wd = inotify_add_watch(fd, "/home/guangtong/pgt_test/inotify_test.dat", IN_MODIFY|IN_CLOSE_WRITE);
    int ffd = open("/home/guangtong/pgt_test/inotify_test.dat", O_RDONLY|O_CREAT|O_NONBLOCK);
    lseek(ffd, -50, SEEK_END);

    struct epoll_event ev, events[MAX_EVENTS];
    int epollfd = epoll_create1(0);
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);

    int nfds, n;
    while(1) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(0);
        }
        for (n=0; n<nfds; ++n) {
            printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
            if (events[n].data.fd == fd) {
                deal_fd(ffd, fd, wd);
                break;
            }
        }

    }





}
