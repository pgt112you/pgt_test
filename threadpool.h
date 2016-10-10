#include <sys/select.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>




typedef struct _task {
  void *(*task_fn)(void *);
  void *task_arg;
  struct _task *next;
} task;


/*
 * 存放线程id的节点
*/
typedef struct _worker {
  pthread_t  tid;
  struct _worker *next;
} worker;

/*
 *  向任务队列中添加任务
*/
void push_task(void *(*task_fn)(void *), void *task_arg);

/*
 * 创建工作线程队列 
*/
void create_worker(int work_num);

/*
 *  销毁线程池
*/
void destroy_pool();

/*
 *  得到当前任务队列中的任务数目
*/
int get_current_task_num();

/*
 *  得到当前工作队列中的线程数目
*/
int get_current_work_num();

/*
 * 初始化线程池
*/
void init_pool();

/*
 *  线程管理器函数
 *  单独的一个线程调用函数
 *  监视worker队列的线程数量
 *  能够自动增加线程
*/
void *create_manager(void *arg);

/*
 *  根据tid删除工作队列中的线程
*/
void delete_worker_by_tid(pthread_t tid);
