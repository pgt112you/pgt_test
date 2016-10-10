#include <dirent.h>
#include <fcntl.h>
#include <openssl/md5.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FILE_DIR "/data0/watchagent_data"


/*
 * 文件节点
*/
typedef struct _fileinfo {
  unsigned char filetype;  // DT_DIR dir, 127 file
  char *filepath;
  char *md5value;
  char operate;    // CURD
  off_t size;
  struct _fileinfo *left;
  struct _fileinfo *right;
  struct _fileinfo *parent;
  int visit;  // 是否被访问过的mark，0否，1是
} fileinfo;


/*
 * 服务端发给客户端的需要更新的结构
*/
typedef struct _result {
  fileinfo *p;  // 服务端二叉查找树中对应的fileinfo节点的值，随客户端的response或者resquest返回
  char md5value[33];
  char filepath[256];
  char operate;
  unsigned char filetype;
} serv_result;

/*
 * 客户端的回执结构
*/
typedef struct _request {
  int type;  // 0 发起更新请求request，1 更新完成结果的response
  fileinfo *p;  // 服务端二叉查找树中对应的fileinfo节点的值
  int result;  // 0 成功， -1 失败
} cli_result;


typedef struct _operatefile {
  fileinfo *op;
  struct _operatefile *next;
} operatefile;




char *file_md5sum(const char *filename);

fileinfo *new_fileinfo(struct dirent *dp, const char *dirpath);

fileinfo *binarytree_s(fileinfo **p, fileinfo *p1, struct dirent *dp, int *mark, const char *dirpath);

void getdirfile(const char *dirpath);

void ldr_binarytree(fileinfo *root);

void generate_serv_result(fileinfo *pf, serv_result *sr);

void add_epoll_event(int epollfd, uint32_t event, int fd);

void del_epoll_event(int epollfd, int fd);

int binarytree_del(fileinfo *p);

fileinfo *searchbst(fileinfo *root, const char *path);

int deletebst(fileinfo *root, const char *path);
