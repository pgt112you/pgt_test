#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

#include <sys/epoll.h>

#define MAX_EVENTS 100

char *sock_addr = "/usr/home/guangtong/test/test.sock";

int main() {
  int sockfd, connfd;
  int n;
  struct sockaddr_un addr1, addr2;
  sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
  //unlink(sock_addr);
  bzero(&addr1, sizeof(addr1));
  addr1.sun_family = AF_LOCAL;
  //strncpy(addr1.sun_path, sock_addr, sizeof(addr1.sun_family)-1);
  strncpy(addr1.sun_path, sock_addr, strlen(sock_addr));
  connect(sockfd, (const struct sockaddr *)&addr1, SUN_LEN(&addr1));
  char *buf[256];
  printf("cli time is %ld\n", time(NULL));
   
  int epollfd;
  int nfds;
  struct epoll_event ev, ev1, events[MAX_EVENTS];
  ev.events = EPOLLIN|EPOLLOUT;
  ev.data.fd = sockfd;

  //ev1.events = EPOLLOUT;
  //ev1.data.fd = sockfd;

  epollfd = epoll_create(10);
  if (epollfd == -1) {
    perror("epoll_create");
    exit(0);
  }
 
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
    perror("epoll_ctl: listen_sock");
    exit(0);
  }

  int j, i = 0;
  /*
  while(i <= 5) {
    ++i;
    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      perror("epoll_pwait");
    }
    printf("nfds is %d\n", nfds);
    for (j = 0; j < nfds; j++) {
      if (events[j].events == EPOLLIN) {
        if ((n = read(events[j].data.fd, buf, 256)) > 0) {
          printf("%d read content is %s", i, buf);
        }
      }
      else if (events[j].events == EPOLLOUT) {
        printf("%d write hello world to server\n", i);
        write(events[j].data.fd, "hello world\n", strlen("hello world\n"));
      }
      else if (events[j].events == EPOLLPRI) {
        printf("%d EPOLLPRI\n", i);
      }
      else if (events[j].events == EPOLLERR) {
        printf("%d EPOLLERR\n", i);
      }
      else if (events[j].events == EPOLLHUP) {
        printf("%d EPOLLHUP\n", i);
      }
      else if (events[j].events == EPOLLET) {
        printf("%d EPOLLET\n", i);
      }
      else if (events[j].events == EPOLLONESHOT) {
        printf("%d EPOLLONESHOT\n", i);
      }
      else {
        printf("events is %x\n", events[j].events);
        perror("epoll wait error ");
        continue;
      }
    }
  }
  */
  write(sockfd, "hello world\n", strlen("hello world\n"));
  printf("cli time is %ld\n", time(NULL));
  nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
  if (nfds == -1) {
    perror("epoll_pwait");
  }
  printf("ndfs is %d\n", nfds);
  n = read(events[0].data.fd, buf, 256);
  if (n <= 0) {
    perror("read error ");
  }
  printf("buf is %s\n", buf);
  close(sockfd);
  return 0;
}
