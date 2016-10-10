/* 
 * 同步两台机器两个文件夹下的文件
 * 每台机器都既是服务端又是客户端
*/
#include <log4cpp/Category.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <unistd.h>
#include "tool.h"
#include "config.h"



pthread_mutex_t sync_lock = PTHREAD_MUTEX_INITIALIZER;
int sync_mark = 0;  // 正在进行同步的标识，0表示没有正在同步，1表示正在进行同步

/*
 * 客户端子进程
*/
void *cli_fn(void *arg) {
  // 初始化日志
  log4cpp::Category &category = getlogger("client.log", "syncfile_client");

  // 监听9112端口，对方发来的请求
  sync_conf *pconf = (sync_conf *)arg;
  std::string rootpath = pconf->syncpath;
  int clifd;
  struct sockaddr_in cliaddr;
  socklen_t cliaddr_len = sizeof(cliaddr);
  int sockfd = create_serv_socket(pconf->servport);
  category.info("create socket listen %d\n", pconf->servport);

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

  std::map<std::string, int> fileopen_map;    // 文件名，打开的文件描述符

  while(1) {
    sync_mark = 0;
    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      perror("epoll_pwait");
      exit(0);
    }
    sync_mark = 1;
    int i;
    int clifd;
    for (i=0; i<nfds; ++i) {
      if (sockfd == events[i].data.fd) {  // 有新的连接
        socklen_t cliaddr_len = sizeof(cliaddr);
        clifd = accept(sockfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
        // 将新连接的sock fd加到epoll的event里面
        add_epoll_event(epollfd, EPOLLIN, clifd);
      }
      else {
        file_trans_data fdata;
        memset(&fdata, '\0', sizeof(fdata));
        bool open_mark;
        //int num = read(events[i].data.fd, &fdata, sizeof(fdata));   这里不能要read，因为socket的缓冲机构可能导致即使没到文件尾，返回的数据仍然比要求的少
        int num = recv(events[i].data.fd, &fdata, sizeof(fdata), MSG_WAITALL);
        if (num <= 0) {
          // 对方关闭了连接，此次对话完成，从epoll的event中去掉此fd
          //del_epoll_event(epollfd, events[i].data.fd);
          close(events[i].data.fd);
          // 解开用来同步的锁
          continue;
        }
        else {
          std::string realpath = rootpath + "/" + fdata.filename;
          // 删除
          if (fdata.operate == 'D') {
            // 删除文件夹
            if (fdata.is_dir == true) {
              int res = rmdir(realpath.c_str());
              if (res == -1) {
                category.warn("delete dir %s err: %s", fdata.filename, strerror(errno));
                continue;
              }
              category.info("delete file %s success", fdata.filename);
            }
            // 删除文件
            else {
              int res = unlink(realpath.c_str());
              if (res == -1) {
                category.warn("delete file %s err: %s", fdata.filename, strerror(errno));
                continue;
              }
              category.info("delete file %s success", fdata.filename);
            }
          }
          // 修改
          else if (fdata.operate == 'U') {
            // 修改文件
            std::map<std::string, int>::iterator it;
            std::string filename = fdata.filename;
            if (fdata.end_mark == true) {  //  文件传输结束
              if ((it = fileopen_map.find(filename)) != fileopen_map.end()) {
                fileopen_map.erase(filename);
                close(it->second);
              }
              else {
                int fd = open(realpath.c_str(), O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
                if (fd != -1) {
                  close(fd);
                }
              }
              // 验证文件的md5值，想服务端的status线程发回传输结果
              char *md5value = file_md5sum(realpath.c_str());
              trans_result fresult;
              memset(&fresult, '\0', sizeof(fresult));
              memcpy(fresult.md5value, md5value, 32);
              memcpy(fresult.filename, fdata.filename, 512);
              free(md5value);
              int clifd = create_cli_socket(pconf->servaddr, pconf->statusport);
              if (clifd < 0) {
                category.warn("to status socket create failed.");
                continue;
              }
              int var = fcntl(clifd, F_GETFD, 0);
              fcntl(clifd, F_SETFD, var|O_NONBLOCK);
              int res = write(clifd, &fresult, sizeof(fresult));
              if (res == -1) {
                category.error("send to status thread error: %s\n", strerror(errno));
              }
              close(clifd);
              category.info("translate file %s over. send md5 value to server", fdata.filename);
              continue;
            }
            if ((it = fileopen_map.find(filename)) != fileopen_map.end()) {
              lseek(it->second, fdata.offset, SEEK_SET);
              write(it->second, fdata.filecontent, fdata.number);
            }
            else {
              int fd = open(fdata.filename, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
              fileopen_map[filename] = fd;
              write(fd, fdata.filecontent, fdata.number);
            }
          }
          // 新建
          else if (fdata.operate == 'C') {
            // 新建文件夹
            if (fdata.is_dir == true) {
              mkdir(realpath.c_str(), S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
              category.info("create dir %s success", fdata.filename);
            }
            // 新建文件
            else {
              int fd = open(realpath.c_str(), O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
              if (fd != -1) {
                close(fd);
              }
            }
          }
          else {
            category.warn("%s other operate %c\n", fdata.filename, fdata.operate);
          }
        }  // end if (num <= 0)  else
      }  // end if (sockfd == events[i].data.fd)  else
    }  // for (i=0; i<nfds; ++i)
  }  // end while(1)
}



/*
 * 服务端子进程
*/
void *serv_fn(void *arg) {
  // 初始化日志
  log4cpp::Category &category = getlogger("server.log", "syncfile_server");

  sync_conf *pconf = (sync_conf *)arg;
  std::string rootpath = pconf->syncpath;
  std::vector<std::string> dirs_vec;
  std::map<int, std::string> wd_path_map;
  int dirfd;
  int len;
  char buffer[EVENT_SIZE*5];
  struct inotify_event *event;
  const char *root = pconf->syncpath;
  get_all_dir(root, dirs_vec);
  dirfd = init_inotify_event();
  std::string root_str = root;
  //dirs_vec.push_back(root);
  std::string rootalias = ".";
  dirs_vec.push_back(rootalias);
  add_inotify_event(dirfd, rootpath, dirs_vec, wd_path_map);
  int i = 0;
  std::string filename;
  int continue_mark = 0;

  char operate = 'C';
  bool is_dir = false;

  while(1) {
    len = read(dirfd, buffer, EVENT_SIZE*5);
    i = 0;
    while(i < len) {
      event = (struct inotify_event *)&buffer[i];
      // 新建
      if (event->mask & (IN_CREATE)) {
        // 新建目录
        if (event->mask & IN_ISDIR) {
          std::vector<std::string> temp_vec;
          std::map<int, std::string>::iterator fit = wd_path_map.find(event->wd);
          if (fit == wd_path_map.end()) {
		    category.warn("create dir %s parent dir not existed", event->name);
            i += EVENT_SIZE + event->len;
            continue_mark = 1;
            continue;
          }
          filename = fit->second + "/" + event->name;
		  category.info("dir %s created", filename.c_str());
          std::string temproot = rootpath + "/" + filename;
		  //sleep(1);  //这里的sleep是为了防止新建文件夹下的子文件下还没有建立起来，得不到其下面的子文件夹，所以sleep等待1秒
          // 得到新目录下的所有文件夹
          get_all_dir(temproot.c_str(), temp_vec);
          temp_vec.push_back(filename);
          // 将新的文件夹以及它的子文件夹加到监控事件里面去
          add_inotify_event(dirfd, temproot, temp_vec, wd_path_map);
          continue_mark = 0;
          is_dir = true;
          operate = 'C';
        }
        // 新建文件
        else {
          std::map<int, std::string>::iterator fit = wd_path_map.find(event->wd);
          if (fit == wd_path_map.end()) {
		    category.warn("create file %s parent dir not existed", event->name);
            i += EVENT_SIZE + event->len;
            continue_mark = 1;
            continue;
          }
          filename = fit->second + "/" + event->name;
		  category.info("file %s created", filename.c_str());
          continue_mark = 0;
          is_dir = false;
          operate = 'C';
        }
      }
      // 删除
      else if (event->mask & (IN_DELETE|IN_MOVED_FROM)) {
        // 删除文件夹
        if (event->mask & IN_ISDIR) {
          std::map<int, std::string>::iterator fit = wd_path_map.find(event->wd);
          if (fit == wd_path_map.end()) {
		    category.warn("delete dir %s not existed", event->name);
            i += EVENT_SIZE + event->len;
            continue_mark = 1;
            continue;
          }
          filename = fit->second + "/" + event->name;
		  category.info("dir %s deleted", filename.c_str());
          // 删除此文件夹的事件，子文件夹不用删除，因为子文件夹删除的事件也会被告知
          //如果此处调用del_inotify_event删除对应的文件夹的事件的话，会造成一个IN_IGNORED的事件产生（因为文件夹已经被删除了，再次inotify_rm_watch就会这样），
          //del_inotify_event(dirfd, event->wd, wd_path_map);
          //文件夹删除对应的wd是上层文件夹的wd，所以不能从map中删掉此wd，不然的话，同目录下的文件删除就不行了
          //wd_path_map.erase(event->wd);
          is_dir = true;
          operate = 'D';
          continue_mark = 0;
        }
        // 删除文件
        else {
          std::map<int, std::string>::iterator fit = wd_path_map.find(event->wd);
          if (fit == wd_path_map.end()) {
		    category.warn("delete file %s not existed", event->name);
            i += EVENT_SIZE + event->len;
            continue_mark = 1;
            continue;
          }
          filename = fit->second + "/" + event->name;
		  category.info("file %s deleted", filename.c_str());
          operate = 'D';
          is_dir = false;
          continue_mark = 0;
        }
      }
      // 修改文件
      else if (event->mask & (IN_CLOSE_WRITE|IN_MOVED_TO)) {
      //else if (event->mask & IN_MODIFY) {
        std::map<int, std::string>::iterator fit = wd_path_map.find(event->wd);
        if (fit == wd_path_map.end()) {
		  category.warn("modify file %s not existed", event->name);
          i += EVENT_SIZE + event->len;
          continue_mark = 1;
          continue;
        }
        filename = fit->second + "/" + event->name;
		category.info("file %s modified", filename.c_str());
        is_dir = false;
        operate = 'U';
        continue_mark = 0;
      }
      // 其他操作
      else {
		category.warn("file %s operate %x wrong", event->name, event->mask);
        continue_mark = 1;
      }
      i += EVENT_SIZE + event->len;


      if (continue_mark) {
        continue_mark = 0;
        continue;
      }
      // 向对方的9112端口，得到服务端文件的改变
      if (sync_mark == 1) {
        continue;
      }

	  int servfd = create_cli_socket(pconf->servaddr, pconf->servport);
	  if (servfd < 0) {
        category.warn("server socket create failed.");
        continue;
	  }
	  trans_data(servfd, filename.c_str(), operate, is_dir);
      continue_mark = 0;
      close(servfd);
    }
  }  
}


void *status_fn(void *arg) {
  // 初始化日志
  log4cpp::Category &category = getlogger("status.log", "syncfile_status");

  // 本身的监听服务，用于获取客户端对文件同步的结果
  sync_conf *pconf = (sync_conf *)arg;
  std::string rootpath = pconf->syncpath;
  int sockfd = create_serv_socket(pconf->statusport); 
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
    for (i=0; i<nfds; ++i) {
      if (sockfd == events[i].data.fd) {  // 有新的连接
	    struct sockaddr cliaddr;
        socklen_t cliaddr_len = sizeof(cliaddr);
        int clifd = accept(sockfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
        // 将新连接的sock fd加到epoll的event里面
        add_epoll_event(epollfd, EPOLLIN, clifd);
      }
	  else {
	    trans_result fresult;
        int num = read(events[i].data.fd, &fresult, sizeof(fresult));
        if (num <= 0) {
          // 对方关闭了连接，此次对话完成，从epoll的event中去掉此fd
          close(events[i].data.fd);
          continue;
        }
		else {
          // 根据对方传过来的文件名和md5值，验证文件传输是否成功
          std::string realpath = rootpath + "/" + fresult.filename;
		  char *md5value = file_md5sum(realpath.c_str());
		  if (md5value == NULL) {
            category.info("file %s md5 value wrong\n", fresult.filename);
            continue;
		  }
		  category.info("file %s md5 value is %.32s", fresult.filename, fresult.md5value);
		  category.info("file %s real md5 value is %s", fresult.filename, md5value);
		  if (memcmp(md5value, fresult.md5value, 32) != 0) {
            // 文件md5值验证失败，重传文件
			int servfd = create_cli_socket(pconf->servaddr, pconf->servport);
	        if (servfd < 0) {
              category.warn("server socket create failed.");
              continue;
	        }
			int var = fcntl(sockfd, F_GETFD, 0);
			fcntl(sockfd, F_SETFD, var|O_NONBLOCK);
            category.info("md5 check failed. retransport %s\n", fresult.filename);
			trans_data(servfd, fresult.filename, 'U', false);
			close(servfd);
		  }
		  else {
            category.info("file %s transport success\n", fresult.filename);
		  }
		  free(md5value);
		}
      }
    }
  }
}

int main() {
  //daemon(0, 0);
  if (already_running()) {
    syslog(LOG_ERR, "daemon already running");
    printf("daemon already running\n");
    exit(1);
  }
  sync_conf sconf;
  get_config(CONF_FILE, &sconf);
  pthread_t serv_tid, cli_tid, status_tid;
  // 开启服务端子线程 
  pthread_create(&serv_tid, NULL, serv_fn, (void *)(&sconf));
  // 开启客户端子线程
  pthread_create(&cli_tid, NULL, cli_fn, (void *)(&sconf));
  // 开启同步状态线程
  pthread_create(&status_tid, NULL, status_fn, (void *)(&sconf));
  pthread_join(serv_tid, NULL);
  pthread_join(cli_tid, NULL);
  pthread_join(status_tid, NULL);
  del_config(&sconf);
}
