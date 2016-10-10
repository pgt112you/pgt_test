#include "threadwork.h"



ThreadWork::ThreadWork(void *(*start_rtn)(void *arg)) {
    process = start_rtn;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
}

int ThreadWork::push_task(void *arg) {
    task_list.push_back(arg);
}

int ThreadWork::destory() {
    task_list.clear();
    pthread_cancel(tid);
}

pthread_t *ThreadWork::get_tid() {
    return &tid;
}

void ThreadWork::start() {
    pthread_create(&tid, &attr, process, this);

}
