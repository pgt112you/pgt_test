/*
 * 测试一下如果直接传输一个结构体
 * 结构体里面有一个指向空的指针，
 * 数据能不能传输过去
*/
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
  //struct trans_data {
  //  int i;
  //  void *p;
  //  //char p[500];
  //};
  //struct trans_data buf;
  //buf.p = malloc(500);
  //bzero(buf.p, 500);
  //int size = sizeof(buf) - sizeof(void *) + 500;
  //printf("sizeof buf is %d\n", sizeof(buf));
  //printf("sizeof void * is %d\n", sizeof(void *));
  //printf("sizeof int is %d\n", sizeof(int));
  void *buf = malloc(sizeof(int)*2+500); 
  bzero(buf, sizeof(int)*2+500);
  int i;
  int j;
  char *str = (char *)malloc(500);
  bzero(str, 500);
  while (1) {
    connfd = accept(sockfd, NULL, NULL);
    //n = read(connfd, &buf, size);
    n = read(connfd, buf, sizeof(int)*2+500);
    memcpy(&i, buf, sizeof(i));
    memcpy(&j, buf+sizeof(i), sizeof(i));
    memcpy(str, buf+sizeof(i)*2, 500);
    printf("buf i is %d\n", i);
    printf("buf j is %d\n", j);
    printf("buf str is %s\n", str);
    printf("buf str is %s\n", buf+sizeof(i)*2);
    close(connfd);
  }
  return 0;
}
