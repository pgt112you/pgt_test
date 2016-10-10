#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#define _SIZE 20
#define LOG_FILE_MODE S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH
//#define LOG_FILE_FLAG O_WRONLY|O_APPEND|O_CREAT
#define LOG_FILE_FLAG O_RDWR|O_CREAT


void parent_func(int fd) {
  struct flock lock;
  lock.l_type = F_WRLCK;
  lock.l_start = 0;
  lock.l_whence = SEEK_SET;
  lock.l_len = 1000;
  lock.l_pid = getpid();
  int res = fcntl(fd, F_SETLK, &lock);
  if (res == -1) {
    perror("parent fcntl error");
  }
  sleep(2);
  char *buf = (char *)malloc(sizeof(char) * 60);
  memset(buf, '\0', 60);

  struct timeval start;
  gettimeofday(&start, NULL);

  int len = sprintf(buf, "parent %ld  %ld %ld\n", getpid(), start.tv_sec, start.tv_usec);
  write(fd, buf, len);
  free(buf);
  sleep(2);
  // 解开record lock
  lock.l_type = F_UNLCK;
  res = fcntl(fd, F_SETLK, &lock);
  if (res == -1) {
    perror("parent fcntl error");
  }
  close(fd);
  return;
}


void child_func(int fd) {

  struct flock lock;
  //lock.l_type = F_WRLCK;
  //lock.l_start = 0;
  //lock.l_whence = SEEK_SET;
  //lock.l_len = 20;
  //lock.l_pid = getpid();
  int res = fcntl(fd, F_GETLK, &lock);
  if (res == -1) {
    perror("child fcntl error");
  }
  printf("lock pid is %d lock whence is %d lock start is %d lock len is %d\n", lock.l_pid, lock.l_whence, lock.l_start, lock.l_len); 
 

  char *buf = (char *)malloc(sizeof(char) * 60);
  memset(buf, '\0', 60);

  struct timeval start;
  gettimeofday(&start, NULL);

  int len = sprintf(buf, "child %ld  %ld %ld\n", getpid(), start.tv_sec, start.tv_usec);
  off_t pos = lseek(fd, 100, SEEK_SET);
  if (pos == -1) {
    perror("lseek error");
  }
  gettimeofday(&start, NULL);
  printf("child before write %d %ld %ld\n", pos, start.tv_sec, start.tv_usec);
  write(fd, buf, len);
  gettimeofday(&start, NULL);
  printf("child after write  %ld %ld\n", start.tv_sec, start.tv_usec);
  free(buf);
  close(fd);
  return;
}


int main() {
  pid_t pid;
  int fd = open("/home/guangtong/pgt_test/temp.txt", LOG_FILE_FLAG, LOG_FILE_MODE);
  if ((pid = fork()) > 0) {    // parent 
    parent_func(fd); 
  }
  else {
    sleep(1);
    child_func(fd);
  }
  return 0;
}
