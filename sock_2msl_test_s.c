/*
 * 测试2msl时间，
 * 测试当服务端处于TIME_WATI状态的时候，
 * 它占用的端口是否能用还
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>

int port = 9112;
int MAX_EVENTS = 10;

int main() {
  int sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in servaddr, cliaddr;
  memset(&servaddr, '\0', sizeof(struct sockaddr_in));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htons(INADDR_ANY);
  servaddr.sin_port = htons(port);
  bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
  listen(sockfd, SOMAXCONN);
  int cliaddr_len = sizeof(cliaddr);
  char *buf = malloc(256);

  int nfds;
  struct epoll_event ev, events[MAX_EVENTS];
  ev.events = EPOLLIN;
  ev.data.fd = sockfd;
  int epollfd = epoll_create(10); 
  if (epollfd == -1) {
    perror("epoll_create");
    exit(0);
  }
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
    perror("epoll_ctl: listen_sock");
    exit(0);
  }

  int i;
  int newfd_mark;
  int num;
  while(1) {
    memset(buf, '\0', 256);
    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      perror("epoll_pwait");
      exit(0);
    }    
    for (i=0; i<nfds; ++i) { 
      int clifd;
      if (events[i].data.fd == sockfd) {    // 有新的连接
        clifd = accept(sockfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
        newfd_mark = 1;
      }
      else {
        clifd = events[i].data.fd;
        newfd_mark = 0;
      }
      num = read(clifd, buf, 256);
      printf("111 num is %dbbbb\n", num);
      if (num <= 0) {  // 对方关闭了连接，将close这个sock，将这个sock对应的event从eventlist中去掉
        printf("in num < 0\n");
        if (epoll_ctl(epollfd, EPOLL_CTL_DEL, clifd, NULL) == -1) {
          perror("epoll_ctl: listen_sock"); 
        }
        printf("close fd %d\n", clifd);
        //close(clifd);
        shutdown(clifd, SHUT_RDWR);
        continue;
      }

      printf("num is %d\n", num);
      write(clifd, buf, 256);
      printf("recv is %s\n", buf);
      if (newfd_mark == 1) {  // 新来的连接，把这个连接增加进eventlist
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = clifd;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, clifd, &ev) == -1) {
          perror("epoll_ctl: listen_sock");
        }
      }
    }
  }
  free(buf);
  close(sockfd);
  return 0;
}
