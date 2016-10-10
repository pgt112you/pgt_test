#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>



typedef struct recv_stru {
  int i;
  int j;
} rs;

typedef struct send_stru {
  int k;
} ss;


int main() {
  int connfd;
  int res;
  struct sockaddr_in servaddr;
  connfd = socket(AF_INET, SOCK_STREAM, 0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  inet_pton(AF_INET, "10.88.15.46", &servaddr.sin_addr);
  servaddr.sin_port = htons(9112);
  res = connect(connfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
  if (res == -1) {
    perror("connect error");
    exit(0);
  }
  rs rsobj = {1,2};
  ss ssobj;
  res = send(connfd, &rsobj, sizeof(rsobj), 0);
  if (res == -1) {
    perror("send error");
  }
  else {
    printf("send %d bytes\n", res);
  }
  char buff[1024];
  recv(connfd, &ssobj, sizeof(ssobj), 0);
  printf("recv content %d\n", ssobj.k);
  return 0;
}
