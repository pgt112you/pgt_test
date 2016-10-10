#include <unistd.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <pthread.h>
#include <list>
#include <string>


#ifndef THREADWORK_H
#define THREADWORK_H


class ThreadTask {
  public:
    ThreadTask(void *(*start_rtn)(void *), void *arg);
    void *(*process) (void *arg);
    void *arg;
};

// thread pool
class ThreadWork { 
  public:
    // task queue
    std::list<void *> task_list; 
    // thread pool destory mark
    int shutdown;
    // current task number in task queue
    int cur_task_size;

    ThreadWork(void *(*start_rtn)(void *arg));

    virtual int push_task(void *arg);
    virtual int destroy();
    virtual pthread_t get_tid();
    virtual start();
  
  private:
    pthread_t = tid;
    void *(*process)(void *arg);
    pthread_attr_t attr;
    
};

#endif


