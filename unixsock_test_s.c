#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

#include <sys/epoll.h>

#define MAX_EVENTS 10

char *sock_addr = "/usr/home/guangtong/test/test.sock";

int main() {
  int sockfd, connfd;
  int n;
  struct sockaddr_un addr1;
  sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
  unlink(sock_addr);
  bzero(&addr1, sizeof(addr1));
  addr1.sun_family = AF_LOCAL;
  //strncpy(addr1.sun_path, sock_addr, sizeof(addr1.sun_family)-1);
  strncpy(addr1.sun_path, sock_addr, strlen(sock_addr));
  int res = bind(sockfd, (const struct sockaddr *)&addr1, SUN_LEN(&addr1));
  if (res < 0) {
    printf("%s\n", strerror(errno));
  }
  listen(sockfd, SOMAXCONN);
  char *buf[256];

  int epollfd, epollfd1;
  int nfds, nfds1;
  struct epoll_event ev, ev1, events[MAX_EVENTS], events1[MAX_EVENTS];
  ev.events = EPOLLIN;
  ev1.events = EPOLLIN;
  epollfd = epoll_create(10);
  if (epollfd == -1) {
    perror("epoll_create");
    exit(0);
  }
  epollfd1 = epoll_create(10);
  if (epollfd1 == -1) {
    perror("epoll_create");
    exit(0);
  }
 
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
    perror("epoll_ctl: listen_sock");
    exit(0);
  }

  ev.data.fd = sockfd;
  int i = 0;
  int j = 0;
  while (i < 100) {
    ++i;
    printf("i is %d\n", i);
    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      printf("=============\n");
      perror("epoll_pwait");
      exit(0);
    }
    printf("time is %ld\n", time(NULL));
    connfd = accept(sockfd, NULL, NULL);
    ev1.data.fd = connfd;
    sleep(2);
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev1) == -1) {
      perror("epoll_ctl: listen_sock");
      exit(0);
    }
      nfds1 = epoll_wait(epollfd, events1, MAX_EVENTS, -1);
      printf("time is %ld\n", time(NULL));
      if ((n = read(connfd, buf, 256)) > 0) {
        printf("read content %s\n", buf);
        //write(connfd, buf, n);
        write(connfd, "jia jie shi\n", strlen("jia jie shi\n"));
      }
      else if (n == 0) {
        printf("other side close\n");
      }
      else {
        printf("socket error %s\n", strerror(errno));
        break;
      }
      close(connfd);
  }
  close(connfd);
  return 0;
}
