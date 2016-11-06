#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>




int main() {
  int listenfd, connfd;
  int res;
  struct sockaddr_in servaddr, cliaddr;
  char buff[1024];
  memset(buff, '\0', 1024);
  listenfd = socket(AF_INET, SOCK_STREAM, 0); 
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(9112);
  res = bind(listenfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
  if (res == -1) {
    perror("bind error");
  }
  listen(listenfd, SOMAXCONN);
  socklen_t cliaddr_len = sizeof(cliaddr);
  connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
  printf("accpet over\n");
  int var = fcntl(connfd, F_GETFD, 0);
  fcntl(connfd, F_SETFD, var|O_NONBLOCK);
  res = recv(connfd, buff, 1024, 0);
  printf("res is %d\n", res);
  return 0;

}
