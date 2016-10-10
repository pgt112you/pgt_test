#include "theadpool.h"




Thread_Pool::Thread_Pool(void *(*process)(void *arg), int thread_num) {
    num = thread_num;
    for (int i=0; i<num; ++i) {
        worker_vec.push_back(ThreadWork(process));
    }
}


Thread_Pool::start() {
    std::vector<ThreadWork>::iterator it;
    for (it = worker_vec.begin(); it != worker_vec.end(); ++it) {
        it->start()
    }
}
