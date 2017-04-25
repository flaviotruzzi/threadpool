//
// Created by Flavio Truzzi on 23.04.17.
//

#ifndef URLSHORTNER_WORKER_H
#define URLSHORTNER_WORKER_H

#include <thread>
#include <atomic>
#include "WorkerQueue.h"

template<typename T>
class WorkerQueue;

template<typename T>
class Worker {
public:
    Worker(WorkerQueue<T> *workerQueue, void (*workerFunction)(T*), std::mutex *mutex);
    void processLoop();
    void finish();
private:
    std::atomic_bool *terminate;
    std::mutex *mutex;
    WorkerQueue<T> *workerQueue;
    void (*workerFunction)(T*);

};


#endif //URLSHORTNER_WORKER_H
