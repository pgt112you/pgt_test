#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10



int main() {
  int n;

  int epollfd;
  int nfds;
  struct epoll_event ev, events[MAX_EVENTS];
  ev.events = EPOLLHUP;
  ev.data.ptr = NULL;
  epollfd = epoll_create(10);
  if (epollfd == -1) {
    perror("epoll_create");
    exit(0);
  }

  while (1) {
    nfds = epoll_wait(epollfd, events, MAX_EVENTS, 5000);
    if (nfds == -1) {
      perror("epoll_pwait");
      exit(0);
    }
    if (nfds == 0) {\
        printf("timeout timeout\n");
    }
  }
  return 0;
}
