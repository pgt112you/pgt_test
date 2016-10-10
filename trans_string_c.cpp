/*
*/
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

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
  struct trans_data {
    int i;
    std::string sa;
    std::string sb;
  };
  struct trans_data buf;
  //buf.i = 5;
  buf.sa = "hello";
  buf.sb = "world";
  buf.i = sizeof(buf.sa) + sizeof(buf.sb);
  write(sockfd, &buf, sizeof(buf));
  close(sockfd);
  return 0;
}
