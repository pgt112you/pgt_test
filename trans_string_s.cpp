/*
 * 测试c++socket能不能
 * 直接发送string对象
*/
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>

#include <sys/epoll.h>


#define MAX_EVENTS 10

char *sock_addr = "/home/guangtong/pgt_test/test.sock";

int main() {
  int sockfd, connfd;
  int n;
  struct sockaddr_un addr1;
  sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
  unlink(sock_addr);
  bzero(&addr1, sizeof(addr1));
  addr1.sun_family = AF_LOCAL;
  strncpy(addr1.sun_path, sock_addr, strlen(sock_addr));
  int res = bind(sockfd, (const struct sockaddr *)&addr1, SUN_LEN(&addr1));
  if (res < 0) {
    printf("%s\n", strerror(errno));
  }
  listen(sockfd, SOMAXCONN);
  struct trans_data {
    int i;
    std::string sa;
    std::string sb;
  };
  struct trans_data buf;
  bzero(&buf, sizeof(buf));
  
  void *buf1 = malloc(256);
  std::string hello;
  while (1) {
    connfd = accept(sockfd, NULL, NULL);
    //n = read(connfd, &buf, size);
    n = read(connfd, buf1, sizeof(buf));
    printf("n is %d\n", n);
    memcpy(&(buf.i), buf1, 8);
    printf("buf i is %d\n", buf.i);
    memcpy(&(buf.sa), buf1+8, 5);
    std::cout << "buf sa is "<<buf.sa << std::endl;
    //printf("buf sa is %s\n", buf.sa.c_str());
    //printf("buf sb is %s\n", buf.sb.c_str());
    close(connfd);
  }
  return 0;
}
