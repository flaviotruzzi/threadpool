//
// Created by Flavio Truzzi on 23.04.17.
//

#include <iostream>
#include "Worker.h"

template <typename T>
Worker<T>::Worker(WorkerQueue<T> *workerQueue,
                  void (*workerFunction)(T*),
                  std::mutex *mutex) {
    this->workerQueue = workerQueue;
    this->workerFunction = workerFunction;
    this->mutex = mutex;
    terminate = new std::atomic_bool(false);
}

template <typename T>
void Worker<T>::finish() {
    terminate->store(true);
}

template <typename T>
void Worker<T>::processLoop() {
    while (!terminate->load()) {
        std::unique_lock<std::mutex> l(*mutex);
        workerQueue->workToBeDone.wait(l);
        l.unlock();

        T* job = workerQueue->getJob();
        while (job != NULL) {
            workerFunction(job);
            job = workerQueue->getJob();
        }
    }

}

template class Worker<int>;