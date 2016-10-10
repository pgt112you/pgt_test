#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <log4cpp/Category.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <openssl/md5.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <syslog.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <map>
#include <string>
#include <vector>

#ifndef TOOL_H
#define TOOL_H

#ifndef SYNC_LOG
#define SYNC_LOG "/home/guangtong/pgt_test/sync_file/log/"
#endif

#ifndef LOCKMODE
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
#endif

#ifndef LOCKFILE
#define LOCKFILE "/home/guangtong/pgt_test/sync_file/pid/syncfile.pid"
#endif

int already_running(void);


log4cpp::Category &getlogger(const char *logfname, const char *instancename);

int get_all_dir(const char *path, std::vector<std::string> &dirs_vec);


void trans_data(int sockfd, const char *filename, char operate, bool is_dir, std::string rootpath);


char *file_md5sum(const char *filename);


void add_epoll_event(int epollfd, uint32_t event, int fd);
void del_epoll_event(int epollfd, int fd);


int init_inotify_event();
int add_inotify_event(int fd, std::string rootpath, std::vector<std::string> &dirs_vec, std::map<int, std::string> &wd_path_map);
int del_inotify_event(int fd, int wd, std::map<int, std::string> &wd_path_map);


int create_cli_socket(const char *servaddr_str, int serv_port); 
int create_serv_socket(int serv_port);

#define MAX_EVENTS 200
#define BLOCK_SIZE 512
#define EVENT_SIZE (sizeof(struct inotify_event))
#define WATCH_EVENTS IN_MOVED_FROM|IN_MOVED_TO|IN_CREATE|IN_DELETE|IN_CLOSE_WRITE


typedef struct _file_trans_data {
  int offset;  // 距离文件开始的偏移量
  char filename[512];  // 文件名，包含路径
  char filecontent[BLOCK_SIZE];  // 文件内容
  int number;  // filecontent中含有的又用的信息的字节数
  bool end_mark;    //  文件是否结束的标识
  char operate;   //  操作CURD
  bool is_dir;    //  是否是文件夹
} file_trans_data;


typedef struct _trans_result {
  char md5value[32];  // 文件的md5值
  char filename[512];  // 更新的文件名
} trans_result;


#endif
