#include "threadpool.h"


/*
 * 任务队列中最大的任务数
 * 当任务数目达到此数目的时候
 * 任务队列将不再添加任务
*/
static int max_task_num;

/*
 *  任务队列的头指针和尾指针
*/
static task *taskq, *taskq_tail;

/*
 * 当前的任务数目
*/
static int current_task_num = 0;

/*
 *  当前的工作线程数目
*/
static int current_work_num = 0;
/*
 * 将任务推到任务队列的函数
*/


/*
 *  工作队列的头指针和尾指针
*/
static worker *workerq, *workerq_tail;


/*
 *  线程池是否被销毁的标识
*/
static bool pool_destory;

pthread_cond_t qready;
pthread_mutex_t taskq_lock;
pthread_mutex_t workerq_lock;


void push_task(void *(*task_fn)(void *), void *task_arg) {
  task *p_newtask = (task*)malloc(sizeof(task));
  p_newtask->task_fn = task_fn;
  p_newtask->task_arg = task_arg;

  pthread_mutex_lock(&taskq_lock);
  if (current_task_num >= max_task_num) {
    // 任务数达到了最大限制
    pthread_mutex_unlock(&taskq_lock);
    return;
  }
  if (current_task_num == 0) {
    taskq = taskq_tail = p_newtask;
    taskq->next = NULL;
  }
  else {
    taskq_tail->next = p_newtask;
    taskq_tail = p_newtask;
    taskq_tail->next = NULL;
  }
  ++current_task_num;
  pthread_mutex_unlock(&taskq_lock);

  pthread_cond_signal(&qready);
  //printf("in push_task after send signal\n");
  return;
}


/*
 * 工作线程的函数
*/
void *work_fn(void *arg) {
  task *task_item;
  while (1) {
    pthread_mutex_lock(&taskq_lock);
    while ((taskq == NULL) && (pool_destory == false)) {
      pthread_cond_wait(&qready, &taskq_lock);
    }
    if (pool_destory == true) {
      pthread_mutex_unlock(&taskq_lock); 
      break;
    }
    task_item = taskq;
    taskq = task_item->next;
    --current_task_num;
    pthread_mutex_unlock(&taskq_lock);
    //printf("in work_fn upcoming to call function\n");
    if (task_item == NULL)
      printf("task_item is null\n");
    //printf("func addr is %x\n", task_item->task_fn);
    (*(task_item->task_fn))(task_item->task_arg);
    //printf("in work_fn after to call function\n");
    free(task_item);
  }
  return NULL;
}


/*
 * 创建工作线程队列 
*/
void create_worker(int work_num) {
  int i;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  for(i=0; i<work_num; ++i) {
    worker *p_worker = (worker *)malloc(sizeof(worker));
    pthread_create(&(p_worker->tid), &attr, work_fn, NULL);
    pthread_mutex_lock(&workerq_lock);
    if (current_work_num == 0) {
      workerq = workerq_tail = p_worker;
      workerq->next = NULL;
    }
    else {
      workerq_tail->next = p_worker;
      workerq_tail = p_worker;
      workerq_tail->next = NULL;
    }
    ++current_work_num;
    pthread_mutex_unlock(&workerq_lock);
  }
  pthread_attr_destroy(&attr);
  return;
}

/*
 *  删除工作线程
 *  此函数不含互斥量操作，如果需要线程同步
 *  需要在抵用此函数的地方，自行加锁
*/
void del_work(worker *delworker) {
  pthread_cancel(delworker->tid);
  free(delworker);
}

/*
 *  增加工作线程
 *  此函数不含互斥量操作，如果需要线程同步
 *  需要在抵用此函数的地方，自行加锁
*/
void add_work() {
  worker *new_worker = (worker *)malloc(sizeof(worker));
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  pthread_create(&(new_worker->tid), &attr, work_fn, NULL);
  workerq_tail->next = new_worker;
  workerq_tail = new_worker;
  return;
}


/*
 *  销毁线程池
*/
void destroy_pool() {
  pool_destory = true;
  pthread_cond_broadcast(&qready);
  // 销毁工作队列
  worker *temp_work;
  pthread_mutex_lock(&workerq_lock);
  while (workerq != NULL) {
    temp_work = workerq;
    workerq = workerq->next;
    free(temp_work);
    --current_work_num; 
  }
  pthread_mutex_unlock(&workerq_lock);
  return;
}


/*
 *  得到当前任务队列中的任务数目
*/
int get_current_task_num() {
  int num = 0;
  pthread_mutex_lock(&taskq_lock);
  num = current_task_num;
  pthread_mutex_unlock(&taskq_lock);
  return num;
}

/*
 *  得到当前工作队列中的线程数目
*/
int get_current_work_num() {
  int num = 0;
  pthread_mutex_lock(&workerq_lock);
  num = current_work_num;
  pthread_mutex_unlock(&workerq_lock);
  return num;
}


/*
 * 初始化线程池
*/
void init_pool() {
  max_task_num = 500;
  pool_destory = false;
  pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
  pthread_mutex_t taskq_lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t workerq_lock = PTHREAD_MUTEX_INITIALIZER;
}


/*
 *  线程管理器函数
 *  单独的一个线程调用函数
 *  监视worker队列的线程数量
 *  能够自动增加线程
*/
void *create_manager(void *arg) {
  struct timeval tv;
  while(1) {
    tv.tv_sec = 60;
    tv.tv_usec = 0;
    select(1, NULL, NULL, NULL, &tv);
    printf("***********in manager thread*******************\n");
    if (pool_destory == true)
      break;
    // 检查worker队列中的线程数目
    worker *p_tempwork;
    int num = 0;
    pthread_mutex_lock(&workerq_lock);
    p_tempwork = workerq;
    while(p_tempwork != NULL) {
      ++num;
      if (p_tempwork->next == NULL)
        break;
      p_tempwork = p_tempwork->next;
    }
    if ((p_tempwork != workerq_tail) || (num != current_work_num)) {
      printf("----in manager thread------\n");
      // 工作队列中的数目不对
      int diff_num;
      worker *pw;
      int i;
      if (num > current_work_num) {
        // 实际的工作线程比记录的多，删掉多余的线程
        printf("num is %d\n", num);
        printf("current_work_num is %d\n", current_work_num);
        diff_num = num - current_work_num;
        for (i=0; i<diff_num; ++i) {
          pw = workerq;
          if (workerq->next != NULL) {
            workerq = workerq->next;
          }
          del_work(pw);
        }
      }
      else if (num < current_work_num) {
        // 实际的工作线程比记录的少，新增加工作线线程
        diff_num = current_work_num - num;
        printf("num is %d\n", num);
        printf("current_work_num is %d\n", current_work_num);
        for (i=0; i<diff_num; ++i) {
          add_work();
          ++current_work_num;
        }
      }
      else {
        printf("workerq is not equal to workerq_tail\n");
      }
    }
    pthread_mutex_unlock(&workerq_lock);
  }
  return NULL;
}

/*
 *  根据tid删除工作队列中的线程
*/
void delete_worker_by_tid(pthread_t tid) {
  //static worker *workerq, *workerq_tail;
  static worker *delwork, *delwork_last; 
  pthread_mutex_lock(&workerq_lock);
  delwork_last = workerq;
  delwork = delwork_last->next;
  while (delwork != NULL) {
    if (pthread_equal(delwork->tid, tid) != 0) {
      // 此线程为要删除的线程
      delwork_last->next = delwork->next;
      del_work(delwork);
      --current_work_num;
      delwork = delwork_last->next;
    }
    else {
      // 此线程不是要删除的线程
      delwork_last = delwork;
      delwork = delwork->next;
    }
  }
  pthread_mutex_unlock(&workerq_lock);
}

