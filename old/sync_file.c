/* 
 * 同步两台机器两个文件夹下的文件
 * 每台机器都既是服务端又是客户端
*/
#include "tool.h"


char *FILE_DIR = "/data0/watchagent_data";
pthread_mutex_lock sync_lock = PTHREAD_MUTEX_INITIALIZER;
int serv_port = 9112;
#define MAX_EVENTS 200



fileinfo *allfile = NULL;  // 二叉查找树的根节点

operatefile *of_list = NULL;  // operate file list此次要传输给对端操作fileinfo对象链表
operatefile *of_list_tail = NULL;  

/*
 * 服务端子进程
*/
void *serv_fn(void *arg) {
  // 监听9112端口，对方发来的请求
  int sockfd, clifd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in servaddr, cliaddr;
  memset(&servaddr, '\0', sizeof(struct sockaddr_in));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htons(INADDR_ANY);
  servaddr.sin_port = htons(serv_port);
  bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
  listen(sockfd, SOMAXCONN);

  int nfds;
  struct epoll_event ev, events[MAX_EVENTS];
  bzero(&ev, sizeof(ev));
  ev.events = EPOLLIN;
  ev.data.fd = sockfd;
  int epollfd = epoll_create(10);
  if (epollfd == -1) {
    perror("epoll_create");
    exit(0);
  }
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
    perror("epoll_ctl: listen_sock");
    exit(0);
  }

  while(1) {
    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      perror("epoll_pwait");
      exit(0);
    }
    int i;
    int clifd;
    for (i=0; i<nfds; ++i) {
      if (sockfd == events[i]) {  // 有新的连接
        // 加上metux lock，同步用的锁，用来保证同一时刻，只有一端作为服务端，一端作为客户端
        pthread_mutex_lock(&sync_lock);
        clifd = accept(sockfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
        // 将新连接的sock fd加到epoll的event里面
        add_epoll_event(epollfd, EPOLLIN, clifd);
        // 得到目前FILE_DIR下的所有的文件和目录
        getdirfile(FILE_DIR);
        // 遍历查找二叉树，得到新增的，修改的，和删除的文件或者文件夹
        ldr_binarytree(allfile);
        // 将要变化的文件信息发送给对方
        while (of_list != NULL) {
          serv_result sr;
          generate_serv_result(of_list, &sr);
          write(clifd, &sr, sizeof(sr));
          fileinfo *temp = of_list;
          of_list = of_list->next;
          free(fileinfo);
        }
        of_list_tail = of_list;
      }
      else {
        cli_result cr;
        int num = read(events[i].fd, &cr, sizeof(cr));
        if (num <= 0) {
          // 对方关闭了连接，此次对话完成，从epoll的event中去掉此fd
          close(events[i].fd)
          del_epoll_event(epollfd, events[i].fd);
          // 解开用来同步的锁
          pthread_mutex_unlock(&sync_lock);
          continue;
        }
        if (cr.type == 0) {
          // 传输文件
          int fd = open(cr.p->filepath, O_RDONLY);
          void *buf = malloc(cr.p->size);
          num = read(fd, buf, cr.p->size);
          num = write(events[i].fd, buf, cr.p->size);
        }
        else {
          // 根据type operate和result来更改二叉查找树
          if (cr.result < 0) {
            // 对方更新失败，不更改查找二叉树中的内容
            continue;
          }
          if (cr.p->operate == 'D') {
            binarytree_del(cr.p);
          }
        }
      }
    }
  }
  
}


/*
 * 客户端子进程
*/
void *cli_fn(void *arg) {
  // 阻塞2分钟，即每2分钟发起一次同步
  sleep(120);
  // 获取用来同步的锁，保证同一时刻，一台机器的服务端和客户端，只能有一个在传输数据
  pthread_mutex_lock(&sync_lock);
  // 通过9113端口，向对方的9112端口，得到服务端文件的改变
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in servaddr, selfaddr;
  memset(&servaddr, '\0', sizeof(struct sockaddr_in));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(9112);
  inet_pton(AF_INET, "10.88.15.94", &servaddr.sin_addr);
  // 客户端自己帮顶端口
  selfaddr.sin_family = AF_INET;
  selfaddr.sin_port = htons(9113);
  selfaddr.sin_addr.s_addr = htons(INADDR_ANY);
  bind(sockfd, (const struct sockaddr *)&selfaddr, sizeof(selfaddr));
  connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

  int nfds;
  struct epoll_event ev, events[MAX_EVENTS];
  bzero(&ev, sizeof(ev));
  ev.events = EPOLLIN;
  ev.data.fd = sockfd;
  int epollfd = epoll_create(10);
  if (epollfd == -1) {
    perror("epoll_create");
    exit(0);
  }
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
    perror("epoll_ctl: listen_sock");
    exit(0);
  }

  while(1) {
    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      perror("epoll_pwait");
      exit(0);
    }
  // 根据服务端文件的改变，修改文件本地的文件（比较文件的最后改变时间，该删除的删除，该更改的和新增的向对方服务端发请求）
    serv_result sr;
    read(sockfd, &sr, sizeof(sr));
    if (sr.operate == 'D') {
      binarytree_del();
    }
  }
  // 解开用来同步的锁
  pthread_mutex_unlock(&sync_lock);
}

int main() {
  pthread_t serv_tid, cli_tid;
  // 开启服务端子线程 
  pthread_create(&serv_tid, NULL, serv_fn, NULL);
  // 开启客户端子进程
  pthread_create(&cli_tid, NULL, serv_fn, NULL);
}
