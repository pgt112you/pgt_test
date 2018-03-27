/*
 * 设置了RCVTIMEO的话，如果超时了，errno是EAGAIN或者EWOULDBLOCK
 * 但是这个时候，系统不会把连接给关了。
 * 然后超时的那条数据不会被丢弃，而是给了后面的read。
 * 这样就会造成了错位，重超时之后，每一个read读到的其实都是前面一个read应该读到的内容提供
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>



int main() {
  int sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in servaddr, selfaddr;
  memset(&servaddr, '\0', sizeof(struct sockaddr_in));
  servaddr.sin_family = AF_INET; 
  servaddr.sin_port = htons(9112);
  inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

  // 客户端自己帮顶端口
  selfaddr.sin_family = AF_INET; 
  selfaddr.sin_port = htons(12306);
  selfaddr.sin_addr.s_addr = htons(INADDR_ANY);
  bind(sockfd, (const struct sockaddr *)&selfaddr, sizeof(selfaddr));

  connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
  char *content = "hello world";
  char *content1 = "hello world11";
  char *content2 = "hello world22";
  char *recvbuf = (char *)malloc(256);
  int n;
  memset(recvbuf, '\0', 256);
  write(sockfd, content, strlen(content));
  n = read(sockfd, recvbuf, 256);
  if (n < 0) {
      printf("000 error send %s\n", content);
      if (errno == EINTR) {
          printf("recv error EINTR\n");
      } else if (errno == EAGAIN) {
          printf("recv error EAGAIN\n");
      } else if (errno == EWOULDBLOCK) {
          printf("recv error EWOULDBLOCK\n");
      }
      sleep(1);
  } else {
      printf("000 send %s, recv %s\n", content, recvbuf);
  }
  bzero(recvbuf, 256);

  write(sockfd, content1, strlen(content1));
  n = read(sockfd, recvbuf, 256);
  if (n < 0) {
      printf("111 error send %s\n", content1);
      if (errno == EINTR) {
          printf("recv error EINTR\n");
      } else if (errno == EAGAIN) {
          printf("recv error EAGAIN\n");
      } else if (errno == EWOULDBLOCK) {
          printf("recv error EWOULDBLOCK\n");
      }
      sleep(1);
  } else {
      printf("111 send %s, recv %s\n", content1, recvbuf);
  }
  bzero(recvbuf, 256);

  write(sockfd, content2, strlen(content2));
  n = read(sockfd, recvbuf, 256);
  if (n < 0) {
      printf("222 error send %s\n", content2);
      if (errno == EINTR) {
          printf("recv error EINTR\n");
      } else if (errno == EAGAIN) {
          printf("recv error EAGAIN\n");
      } else if (errno == EWOULDBLOCK) {
          printf("recv error EWOULDBLOCK\n");
      }
      sleep(1);
  } else {
      printf("222 send %s, recv %s\n", content2, recvbuf);
  }
  bzero(recvbuf, 256);

  sleep(5);
  close(sockfd);
  return 0;
}
