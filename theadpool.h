#include <vector>
#include "threadwork.h"


#ifndef THREADPOOL_H
#define THREADPOOL_H

class Thread_Pool {
  public:
    Thread_Pool(void *(*process)(void *arg), int num)
    void start();

  private:
    int num;    // worker thread number
    std::vector<ThreadWork> worker_vec; 

};







#endif






