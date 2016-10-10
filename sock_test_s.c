#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>


typedef struct recv_stru {
  int i;
  int j;
} rs;

typedef struct send_stru {
  int k;
} ss;


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
  rs rsobj;
  ss ssobj;
  while(1) {
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
    recv(connfd, &rsobj, sizeof(rsobj), 0);
    printf("recv content %d  %d\n", rsobj.i, rsobj.j);
    ssobj.k = rsobj.i + rsobj.j;
    send(connfd, &ssobj, sizeof(ssobj), 0);
  }
  return 0;

}
