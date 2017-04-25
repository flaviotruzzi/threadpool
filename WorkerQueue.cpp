//
// Created by Flavio Truzzi on 23.04.17.
//

#include <iostream>
#include "WorkerQueue.h"


template<typename T>
WorkerQueue<T>::WorkerQueue(int poolSize, int queueSize, void (*workerFunction)(T *)) {
    this->poolSize = poolSize;
    this->queueSize = queueSize;


    for (int i = 0; i < this->poolSize; ++i) {
        Worker<T> *worker = new Worker<T>(this, workerFunction, &workMutex);
        workers.push_back(worker);

        std::thread *thread = new std::thread(&Worker<T>::processLoop, *worker);

        threads.push_back(thread);

    }

}

template<typename T>
T *WorkerQueue<T>::getJob() {
    std::lock_guard<std::mutex> lock(queue_mutex);

    T *job = NULL;

    if (!jobs.empty()) {
        job = jobs.front();
        jobs.pop();
        workToBeDone.notify_one();
    }

    return job;
}

template<typename T>
bool WorkerQueue<T>::addJob(T *job) {
    bool result = false;

    if (jobs.size() <= queueSize) {
        jobs.push(job);
        result = true;
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            workToBeDone.notify_all();
        }
    }

    return result;
}

template<typename T>
void WorkerQueue<T>::finish() {

    for (int i = 0; i < poolSize; ++i) {
        workers[i]->finish();
    }

    workToBeDone.notify_all();

    for (int i = 0; i < poolSize; ++i) {
        threads[i]->join();
    }


}


template
class WorkerQueue<int>;