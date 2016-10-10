#include <dirent.h>
#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/inotify.h>
#include <map>
#include <string>
#include <vector>

//std::vector<std::string> dirs_vec;
//std::map<int, std::string> wd_path_map;

#define EVENT_SIZE (sizeof(struct inotify_event))

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
      printf("11dir %s\n", dirpath.c_str());
    }

  }
  return 0;
}

#define WATCH_EVENTS IN_MODIFY|IN_MOVED_FROM|IN_MOVED_TO|IN_CREATE|IN_DELETE

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
int add_inotify_event(int fd, std::vector<std::string> &dirs_vec, std::map<int, std::string> &wd_path_map) {
  // 添加事件
  for (std::vector<std::string>::iterator it = dirs_vec.begin() ; it != dirs_vec.end(); ++it) {
    printf("dir %s\n", it->c_str());
    int wd = inotify_add_watch(fd, it->c_str(), WATCH_EVENTS);
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


int main() {
  std::vector<std::string> dirs_vec;
  std::map<int, std::string> wd_path_map;
  int fd;
  int len;
  char buffer[EVENT_SIZE*5];
  struct inotify_event *event;
  const char *root = "/data0/watchagent_data";
  get_all_dir(root, dirs_vec);  
  fd = init_inotify_event();
  std::string root_str = root;
  dirs_vec.push_back(root);
  add_inotify_event(fd, dirs_vec, wd_path_map);
  int i = 0;
  while(1) {
    len = read(fd, buffer, EVENT_SIZE*5);
    printf("len is %d\n", len);
    printf("inotify event is %d\n", EVENT_SIZE);
    i = 0;
    while(i < len) {
      printf("i is %d\n", i);
      event = (struct inotify_event *)&buffer[i];
      if (event->mask & (IN_CREATE|IN_MOVED_FROM)) {
        if (event->mask & IN_ISDIR) {
          printf("dir %s created\n", event->name);
          std::vector<std::string> temp_vec;
          std::map<int, std::string>::iterator fit = wd_path_map.find(event->wd);
          if (fit == wd_path_map.end()) {
            printf("path wrong\n");
            i += EVENT_SIZE + event->len;
            continue;
          }
          std::string temproot = fit->second + "/" + event->name;
          // 得到新目录下的所有文件夹
          get_all_dir(temproot.c_str(), temp_vec);
          temp_vec.push_back(temproot);
          // 将新的文件夹以及它的子文件夹加到监控事件里面去
          add_inotify_event(fd, temp_vec, wd_path_map);
        }
        else {
          printf("file %s created\n", event->name);
        }
      }
      else if (event->mask & (IN_DELETE|IN_MOVED_TO)) {
        if (event->mask & IN_ISDIR) {
          printf("dir %s deleted\n", event->name);
          // 删除此文件夹的事件，子文件夹不用删除，因为子文件夹删除的事件也会被告知
          printf("wd is %ld\n", event->wd);
          //如果此处调用del_inotify_event删除对应的文件夹的事件的话，会造成一个IN_IGNORED的事件产生（因为文件夹已经被删除了，再次inotify_rm_watch就会这样），
          //del_inotify_event(fd, event->wd, wd_path_map);
          wd_path_map.erase(event->wd);
        }
        else {
          printf("file %s deleted\n", event->name);
        }
      }
      else if (event->mask & IN_MODIFY) {
        printf("file %s modified\n", event->name);
      }
      else {
        printf("wrong event, name is %s, mask is %x\n", event->name, event->mask);
      }
      i += EVENT_SIZE + event->len;
      printf("event len is %d\n", event->len);
    }
  }
}
