#include "tool.h"



// make a lock on file which descriptor is fd
int lockfile(int fd) {
  struct flock fl;
  fl.l_type = F_WRLCK;
  fl.l_start = 0;
  fl.l_whence = SEEK_SET;
  fl.l_len = 0;
  return (fcntl(fd, F_SETLK, &fl));
}


// make the daemon has only one active process at any time
int already_running(void) {
  int fd;
  char buf[16];

  fd = open(LOCKFILE, O_RDWR|O_CREAT, LOCKMODE);
  if (fd < 0) {
    syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
    exit(1);
  }
  if (lockfile(fd) < 0) {
    if (errno == EACCES || errno == EAGAIN) {
      close(fd);
      return(1);
    }
    syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
    exit(1);
  }
  ftruncate(fd, 0);
  sprintf(buf, "%ld", (long)getpid());
  write(fd, buf, strlen(buf)+1);
  //close(fd); 关了之后errno就不是eacces和eagain了
  return(0);
}



/*
 * 得到日志实例对象
*/
log4cpp::Category &getlogger(const char *logfname, const char *instancename) {
  std::string const logfilename = std::string(SYNC_LOG) + logfname;
  printf("log file is %s\n", logfilename.c_str());
  log4cpp::Category &category = log4cpp::Category::getInstance(instancename);
  struct log4cpp::RollingFileAppender::RollingFileAppender *appender = new struct log4cpp::RollingFileAppender::RollingFileAppender
("synfile", logfilename, 10*1024*1024, 9);
  log4cpp::PatternLayout *layout = new log4cpp::PatternLayout();
  layout->setConversionPattern("%p %d{%Y-%m-%d %H:%M:%S} %x: %m%n");
  appender->setLayout(layout);
  category.setAppender(appender);
  category.setPriority(log4cpp::Priority::DEBUG);
  return category;
}


/*
 * 得到存储目录下的所有的文件夹
*/
int get_all_dir(const char *path, std::vector<std::string> &dirs_vec) {
  DIR *dirp = opendir(path);
  if (dirp == NULL) {
    return -1;
  }

  struct dirent *dp;
  int mark;
  while ((dp = readdir(dirp)) != NULL) {
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
      continue;
    }
    if (dp->d_type == DT_DIR) {
      std::string dirpath = std::string(path) + "/" + dp->d_name;
      dirs_vec.push_back(dirpath);
      get_all_dir(dirpath.c_str(), dirs_vec);
    }

  }
  return 0;
}

void trans_data(int sockfd, const char *filename, char operate, bool is_dir, std::string rootpath) {
  int num;
  int var = fcntl(sockfd, F_GETFD, 0);
  fcntl(sockfd, F_SETFD, var|O_NONBLOCK);
  if (operate == 'U') {
    if (!is_dir) {
      char *buf = (char *)malloc(BLOCK_SIZE);
      std::string realpath = rootpath + "/" + filename;
      int fd = open(realpath.c_str(), O_RDONLY);
      int offset = 0;
      while ((num = read(fd, buf, BLOCK_SIZE)) > 0) {
        file_trans_data fdata;
        memset(&fdata, '\0', sizeof(fdata));
        fdata.offset = offset;
        strcpy(fdata.filename, filename);
        memcpy(fdata.filecontent, buf, BLOCK_SIZE);
        fdata.number = num;
        fdata.end_mark = false;
        fdata.operate = operate;
        fdata.is_dir = is_dir;
        write(sockfd, &fdata, sizeof(fdata));
        offset += num;
      }
      close(fd);
      free(buf);
      if (num == -1) {
        printf("read error: %s\n", strerror(errno));
      }
    }
  }
  file_trans_data fdata;
  memset(&fdata, '\0', sizeof(fdata));
  strcpy(fdata.filename, filename);
  fdata.operate = operate;
  fdata.is_dir = is_dir;
  fdata.end_mark = true;
  write(sockfd, &fdata, sizeof(fdata));
  return ;
}


/*
 * 计算文件的md5值
*/
char *file_md5sum(const char *filename) {
  MD5_CTX c;
  MD5_Init(&c);
  FILE *f;
  f = fopen(filename, "r");
  if (f == NULL) return NULL;
  unsigned char md[MD5_DIGEST_LENGTH];
  void *pbuf = malloc(2048);
  size_t res;
  while (1) {
    res = fread(pbuf, 1, 2048, f); 
    if (res <= 0) {
      break;
    }   
    MD5_Update(&c, pbuf, res);
  }
  MD5_Final(md, &c);
  free(pbuf);
  fclose(f); 
  char *result = (char *)malloc(sizeof(char)*32);
  memset(result, '\0', 32);
  int i; 
  for(i = 0; i< MD5_DIGEST_LENGTH; i++) {
    sprintf(result+2*i, "%02x", md[i]);
  }
  return result;
}


// ==================epoll=============================

void add_epoll_event(int epollfd, uint32_t event, int fd) {
  struct epoll_event ev; 
  ev.events = event; 
  ev.data.fd = fd;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
    perror("add epoll_ctl: listen_sock");
  } 
} 

void del_epoll_event(int epollfd, int fd) {
  if (epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
    perror("del epoll_ctl: listen_sock");
  } 
} 


// ==================inotify=============================

/*
 * 初始化inotify事件
*/
int init_inotify_event() {
  // 初始化inotify
  int fd = inotify_init();
  if (fd < 0) {
    printf("Fail to initialize inotify.\n");
    return -1;
  }
  return fd;
}

/*
 * 新增文件夹，添加inotify监控的文件夹
*/
int add_inotify_event(int fd, std::string rootpath, std::vector<std::string> &dirs_vec, std::map<int, std::string> &wd_path_map) {
  // 添加事件
  for (std::vector<std::string>::iterator it = dirs_vec.begin() ; it != dirs_vec.end(); ++it) {
    std::string temppath = rootpath + "/" + *it;
    int wd = inotify_add_watch(fd, rootpath.c_str(), WATCH_EVENTS);
    if (wd == -1) {
      printf("add err %s\n", strerror(errno));
    }
    wd_path_map[wd] = *it;
  }
  return 0;
}

/*
 * 删除文件夹，取消inotify监控的文件夹
*/
int del_inotify_event(int fd, int wd, std::map<int, std::string> &wd_path_map) {
  inotify_rm_watch(fd, wd);
  wd_path_map.erase(wd);
  return 0;
} 



// ==================socket=============================

int create_cli_socket(const char *servaddr_str, int serv_port) {
  int servfd;
  servfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in servaddr, selfaddr;
  memset(&servaddr, '\0', sizeof(struct sockaddr_in));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(serv_port);
  inet_pton(AF_INET, servaddr_str, &servaddr.sin_addr);
  int res = connect(servfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
  if (res == -1) {
    return -1;
  }
  return servfd;
}


int create_serv_socket(int serv_port) {
  int servfd;
  servfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in servaddr;
  memset(&servaddr, '\0', sizeof(struct sockaddr_in));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htons(INADDR_ANY);
  servaddr.sin_port = htons(serv_port);
  bind(servfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
  int res = listen(servfd, SOMAXCONN);
  if (res == -1) {
    return -1;
  }
  return servfd;
}


