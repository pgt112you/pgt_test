/*
*/
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

#include <sys/epoll.h>

#define MAX_EVENTS 100

char *sock_addr = "/home/guangtong/pgt_test/test.sock";

int main() {
  int sockfd, connfd;
  int n;
  struct sockaddr_un addr1;
  sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
  bzero(&addr1, sizeof(addr1));
  addr1.sun_family = AF_LOCAL;
  strncpy(addr1.sun_path, sock_addr, strlen(sock_addr));
  connect(sockfd, (const struct sockaddr *)&addr1, SUN_LEN(&addr1));
  //struct trans_data {
  //  int i;
  //  void *p;
  //  char p[5];
  //};
  //struct trans_data buf;
  //memset(&buf, '\0', sizeof(buf));
  //buf.i = 5;
  //char *str = "hello world";
  ////buf.p = str;
  //strncpy(buf.p, str, 4);
  //buf.p[4] = '\0';
  //printf("buf i is %d\n", buf.i);
  //printf("buf p is %s\n", (char *)buf.p);
  void *buf = malloc(sizeof(int)*2+500); 
  bzero(buf, sizeof(int)*2+500);
  int i = 5;
  int j = 6;
  char *str = "hello world";
  memcpy(buf, &i, sizeof(i));
  memcpy(buf+sizeof(i), &j, sizeof(j));
  memcpy(buf+sizeof(i)*2, str, strlen(str)+1);
  printf("str is %s\n", buf+sizeof(i)*2);
  write(sockfd, buf, 500+sizeof(i)*2);
  close(sockfd);
  return 0;
}
