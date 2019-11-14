/*
 * 测试setsockopt 在连接
 * 建立之后调用,是否起作用
 * 此程序是测试的服务端程序
 * 客户端程序使用telnet即可
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>

int port = 9122;
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

  int num;
  while(1) {
    int clifd = accept(sockfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
    struct linger lop;
    lop.l_onoff = 1;
    lop.l_linger = 0;
    // 加上下面这一样，客户端就会报错 Connection reset by peer, 不加的话，就是正常的close
    setsockopt(clifd,SOL_SOCKET,SO_LINGER,(const char*)&lop,sizeof(struct linger));
    num = read(clifd, buf, 256);
    close(clifd);
    //if (num <= 0) {  // 对方关闭了连接，将close这个sock，将这个sock对应的event从eventlist中去掉
    //  printf("in num < 0\n");
    //  printf("close fd %d\n", clifd);
    //  close(clifd);
    //}
    //printf("recv %s\n", buf);

    //printf("sleep 2 second, ii is %d\n", ii);
    sleep(1);

  }
  free(buf);
  close(sockfd);
  return 0;
}
