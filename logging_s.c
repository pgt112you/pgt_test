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
#define READ_SIZE 4096
#define LOG_FILE_MODE S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IROTH
#define LOG_FILE_FLAG O_WRONLY|O_APPEND|O_CREAT

char *SOCK_ADDR = "/home/guangtong/pgt_test/test.sock";

typedef struct thread_arg_s {
  int fd;
  int logfd;
} thread_arg;

void *writelog(void* arg) {
  thread_arg *temp = (thread_arg *)arg;
  int clisock = temp->fd;
  printf("socket is %d\n", clisock);
  char *buf = (char *)malloc(sizeof(char)*READ_SIZE);
  memset(buf, '\0', READ_SIZE);
  int epollfd;
  int nfds;
  struct epoll_event ev, events[1];
  ev.events = EPOLLIN;
  ev.data.fd = clisock;
  epollfd = epoll_create(10);
  if (epollfd == -1) {
    perror("epoll_create 1");
    exit(0);
  }
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, clisock, &ev) == -1) {
    perror("epoll_ctl 1: listen_sock");
    exit(0);
  }
  while (1) {
    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      perror("epoll_pwait 1");
      return NULL;
    }
    memset(buf, '\0', READ_SIZE);
    int r_size = read(clisock, buf, READ_SIZE);
    if(r_size < 0) {
      perror("read error "); 
      break;
    }
    if(r_size == 0) {  // 如果对方中断了连接，epoll_wait中的事件也会变成可读，只不过这次读到的数据大小为0
      printf("no data read\n");
      break;
    }

    struct timeval start;
    gettimeofday(&start, NULL);
    printf("%ld    %ld %ld\n", (long)pthread_self(), start.tv_sec, start.tv_usec);
    printf("%ld read %d size\n", (long)pthread_self(), r_size);

    if (write(temp->logfd, buf, r_size) == -1) {
      perror("fwrite error "); 
    }
  }
  close(clisock);
  free(buf);
}

int main() {
  int sockfd, connfd;
  int n;
  struct sockaddr_un addr1;
  sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
  unlink(SOCK_ADDR);
  bzero(&addr1, sizeof(addr1));
  addr1.sun_family = AF_LOCAL;
  strncpy(addr1.sun_path, SOCK_ADDR, strlen(SOCK_ADDR));
  int res = bind(sockfd, (const struct sockaddr *)&addr1, SUN_LEN(&addr1));
  if (res < 0) {
    printf("%s\n", strerror(errno));
    return 0;
  }
  listen(sockfd, SOMAXCONN);
  char *buf[256];

  int epollfd;
  int nfds;
  struct epoll_event ev, events[MAX_EVENTS];
  ev.events = EPOLLIN;
  ev.data.fd = sockfd;
  epollfd = epoll_create(10);
  if (epollfd == -1) {
    perror("epoll_create");
    exit(0);
  }
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
    perror("epoll_ctl: listen_sock");
    exit(0);
  }
  
  int i;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  int logfd = open("/home/guangtong/pgt_test/log_s.log", LOG_FILE_FLAG, LOG_FILE_MODE);
  if (logfd == -1) {
    perror("open log file error");
    exit(0);
  }
  while (1) {
    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      perror("epoll_pwait");
      exit(0);
    }
    printf("nfds is %d\n", nfds);
    for (i=0; i<nfds; ++i) {
      thread_arg *p_ta = (thread_arg *)malloc(sizeof(thread_arg));
      p_ta->fd = accept(events[i].data.fd, NULL, NULL);
      p_ta->logfd = logfd;
      pthread_t pid;
      pthread_create(&pid, &attr, writelog, (void *)p_ta);
    }
  }
  pthread_attr_destroy(&attr);
  close(connfd);
  close(logfd);
  return 0;
}
