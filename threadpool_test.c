#include "threadpool.h"

/*
 *  任务函数
*/
void *task_fn(void *arg) {
  printf("==in task_fn== %ld %ld\n", (int)pthread_self(), time(NULL));
  return;
}


int main() {
  pthread_t manager_tid;
  // 初始化线程池
  printf("init pool\n");
  init_pool();
  // 创建工作线程的队列
  printf("create worker\n");
  create_worker(20);
  // 创建管理线程
  printf("create manager thread\n");
  pthread_create(&manager_tid, NULL, create_manager, NULL);
  // 创建任务函数和任务参数
  // 将任务推入到工作队列
  printf("create task func and task arguemnt\n");
  int i;
  while(1) {
    struct timeval tv; 
    tv.tv_sec = 6;
    tv.tv_usec = 0;
    select(1, NULL, NULL, NULL, &tv);
    for (i=0; i<8; ++i) {
      printf("push task %ld %d\n", time(NULL), i);
      push_task(task_fn, NULL);
    }
  printf("task num is %d\n", get_current_task_num());
  printf("work num is %d\n", get_current_work_num());
  }
  pthread_join(manager_tid, NULL);
  return;
}
