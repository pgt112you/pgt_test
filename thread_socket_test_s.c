#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


/*
 * 同一个进程（服务端进程），一个线程往一个socket fd里面写数据
 * 另一个线程从同一个fd里面读取数据，是不会度乱掉的
 * 就是写线程写给对端的数据不会被读线程给读取出来
*/

void *read_func(void *arg) {
    int fd = *(int *)arg;
    char buf[256];
    bzero(buf, 256);
    int res = read(fd, buf, 256);
    printf("read thread read len is %d, content is %s\n", res, buf);
    return NULL;
}

void *write_func(void *arg) {
    int fd = *(int *)arg;
    write(fd, "hello world", strlen("hello world"));
    printf("write thread write content is \"hello world\"\n");
    return NULL;
}



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
  char buf[256];
  bzero(buf, 256);
  while(1) {
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
    recv(connfd, buf, 256, 0);
    printf("recv content %s\n", buf);
    pthread_t r_t, w_t;
    sleep(3);
    pthread_create(&r_t, NULL, read_func, (void *)&connfd);
    pthread_create(&w_t, NULL, write_func, (void *)&connfd);
    sleep(200);
  }
  return 0;

}
