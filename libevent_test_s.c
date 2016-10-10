#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <event2/event-config.h> 
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <time.h>
//#include <event2/event_struct.h> 


int port = 9910;


void ev_cb(evutil_socket_t fd, short event, void *arg) {
  printf("hello world\n");
  struct sockaddr_in cliaddr;
  socklen_t clilen = sizeof(cliaddr);
  int cfd = accept(fd, (struct sockaddr *)&cliaddr, &clilen);
  char buf[100];
  read(cfd, buf, 100);
  printf("recv data is %s\n", buf);
  write(cfd, "hello world!", strlen("hello world!"));
  //close(cfd);
  return;
}




int main() {
  struct event *test_ev;
  //struct event test_ev;
  struct event_base *base;
  struct sockaddr_in sin;
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  int sfd = socket(AF_INET, SOCK_STREAM, 0); 
  bind(sfd, (struct sockaddr *)&sin, sizeof(sin));
  listen(sfd, 10);
  base = event_base_new();
  test_ev = event_new(base, sfd,  EV_READ|EV_PERSIST, ev_cb, NULL);
  event_add(test_ev, NULL);
  //event_assign(&test_ev, base, sfd,  EV_READ, ev_cb, NULL);
  event_base_dispatch(base);
  printf("aaa\n");
  return 0;
}
