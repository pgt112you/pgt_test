#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <event2/event-config.h> 
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/event_struct.h> 
#include <time.h>


int port = 9910;


void ev_cb(evutil_socket_t fd, short event, void *arg) {
  printf("hello world\n");
  struct sockaddr_in cliaddr;
  socklen_t clilen = sizeof(cliaddr);
  //int cfd = accept(fd, (struct sockaddr *)&cliaddr, &clilen);
  char buf[100];
  read(fd, buf, 100);
  close(fd);
  printf("buf is %s\n", buf);
  return;
}

void ev_cb1(evutil_socket_t fd, short event, void *arg) {
  printf("jiajieshi\n");
  FILE *ff = (FILE *)arg;
  struct sockaddr_in cliaddr;
  char buf1[1000];
  fread(buf1, sizeof(char), 1000, ff);
  printf("buf1 is %s\n", buf1);
  fclose(ff);
  return;
}



int main() {
  struct event *test_ev, *test_ev1;
  //struct event test_ev;
  struct event_base *base;
  struct sockaddr_in sin;
  //sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr);
  //inet_pton(AF_INET, "10.88.15.69", &sin.sin_addr);
  int sfd = socket(AF_INET, SOCK_STREAM, 0); 
  connect(sfd, (struct sockaddr *)&sin, sizeof(sin)); 
  int val = fcntl(sfd, F_GETFL, 0);
  fcntl(sfd, F_SETFL, val|O_NONBLOCK);
  printf("time -1 is %ld\n", time(NULL));
  write(sfd, "nihao shijie");
  printf("time 0 is %ld\n", time(NULL));
  FILE *f = popen("ping -c4 10.210.128.53", "r");
  int sfd1 = fileno(f);
  printf("time 1 is %ld\n", time(NULL));
  base = event_base_new();
  test_ev = event_new(base, sfd,  EV_READ, ev_cb, NULL);
  test_ev1 = event_new(base, sfd1,  EV_READ, ev_cb1, (void*)f);
  event_add(test_ev, NULL);
  event_add(test_ev1, NULL);
  sleep(1);
  //event_assign(&test_ev, base, sfd,  EV_READ, ev_cb, NULL);
  event_base_dispatch(base);
  printf("bbb\n");
  return 0;
}
