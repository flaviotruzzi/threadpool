//
// Created by Flavio Truzzi on 23.04.17.
//

#ifndef URLSHORTNER_WORKERBASE_H
#define URLSHORTNER_WORKERBASE_H

#include <thread>
#include <atomic>
#include <functional>
#include "ThreadPool.h"

template<class Work>
class WorkerBase {
public:
    WorkerBase(std::function<Work *(void)> getWork_,
               std::mutex &mutex_,
               std::condition_variable &cv_);

    void finish();

private:
    std::thread thisThread;
    std::atomic<bool> terminate;
    std::mutex &mutex;
    std::condition_variable &cv;
    std::function<Work *(void)> getWork;

    void processLoop();


    virtual void work(Work *work) = 0;
};


template<class Work>
void WorkerBase<Work>::processLoop() {
    while (!terminate.load()) {
        std::unique_lock<std::mutex> l(mutex);
        cv.wait(l);
        l.unlock();

        Work *job = getWork();
        while (job != NULL) {
            work(job);
            job = getWork();
        }
    }

    thisThread.join();

}


template<class Work>
WorkerBase<Work>::WorkerBase(std::function<Work *(void)> getWork_,
                             std::mutex &mutex_,
                             std::condition_variable &cv_)
        : getWork(getWork_), mutex(mutex_), cv(cv_) {
    terminate.store(false);
    thisThread = std::thread(&WorkerBase::processLoop, this);
}

template<class Work>
void WorkerBase<Work>::finish() {
    terminate.store(true);
}


#endif //URLSHORTNER_WORKER_H
