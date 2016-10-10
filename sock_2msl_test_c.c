/*
 * 测试2msl时间
 * 测试当服务端处于TIME_WATI状态的时候，
 * 它占用的端口是否能用还
 *
 * 结论：
 * 第一次运行test_c，然后sudo netstat -npa | grep 9112，可以看到
 * tcp        0      0 0.0.0.0:9112                0.0.0.0:*                   LISTEN      10344/./test_s      
 * tcp        0      0 127.0.0.1:12306             127.0.0.1:9112              TIME_WAIT   - 
 * 这说明已经在TIME_WAIT状态了，并且过了一段时间之后第二行TIME_WAIT没有了，说明过了2msl了。
 * 如果连续运行两次test_c，发现第二次运行的时候，客户端的端口变了
 * tcp        0      0 0.0.0.0:9112                0.0.0.0:*                   LISTEN      10344/./test_s      
 * tcp        0      0 127.0.0.1:59667             127.0.0.1:9112              TIME_WAIT   -                   
 * tcp        0      0 127.0.0.1:12306             127.0.0.1:9112              TIME_WAIT   -   
 * 说明linux不是像tcp/ip书里说的那样，没法运行，而是强制换了一个端口，即使bind了一个端口也没用
 * 
 * 同样的
 * 如果在客户端的两次发送数据之间加上sleep(5)，同时在这期间，将服务端ctrl+c掉，然后在重新启动，结果发现还能启动起来，但是如果用netstat看一下会发现
 * tcp        0      0 0.0.0.0:56321               0.0.0.0:*                   LISTEN      10781/./test_s  
 * 说明服务端端口号也会变，不管bind和listen的是哪个
 * 奇怪的是在把服务端ctrl+c之后，sleep之后的write的信息还能还能被服务端再给回显回来，不知道是为什么。
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



int main() {
  int sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in servaddr, selfaddr;
  memset(&servaddr, '\0', sizeof(struct sockaddr_in));
  servaddr.sin_family = AF_INET; 
  servaddr.sin_port = htons(9112);
  inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
  // 客户端自己帮顶端口
  selfaddr.sin_family = AF_INET; 
  selfaddr.sin_port = htons(12306);
  selfaddr.sin_addr.s_addr = htons(INADDR_ANY);
  bind(sockfd, (const struct sockaddr *)&selfaddr, sizeof(selfaddr));

  connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
  char *content = "hello world\n";
  char *recvbuf = (char *)malloc(256);
  memset(recvbuf, '\0', 256);
  write(sockfd, content, strlen(content));
  read(sockfd, recvbuf, 256);
  printf("000 %s", recvbuf);
  //sleep(10);
  //memset(recvbuf, '\0', 256);
  //write(sockfd, content, strlen(content));
  //read(sockfd, recvbuf, 256);
  //printf("%s", recvbuf);
  close(sockfd);
  return 0;
}
