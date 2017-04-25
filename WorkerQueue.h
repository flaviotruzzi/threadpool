//
// Created by Flavio Truzzi on 23.04.17.
//

#ifndef URLSHORTNER_WORKERQUEUE_H
#define URLSHORTNER_WORKERQUEUE_H

#include <queue>
#include <thread>
#include "WorkerQueue.h"
#include "Worker.h"

template <typename T>
class Worker;

template <typename T>
class WorkerQueue {
public:
    WorkerQueue(int poolSize, int queueSize, void (*workerFunction)(T*));
    T *getJob();
    void finish();
    bool addJob(T *job);
    std::queue<T*> jobs;
    std::condition_variable workToBeDone;
private:
    std::mutex workMutex;
    std::mutex queue_mutex;
    std::vector<std::thread*> threads;
    std::vector<Worker<T>*> workers;

    int poolSize;
    int queueSize;

};



#endif //URLSHORTNER_WORKERQUEUE_H
