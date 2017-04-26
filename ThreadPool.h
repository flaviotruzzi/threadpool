//
// Created by Flavio Truzzi on 23.04.17.
//

#ifndef URLSHORTNER_THREADPOOL_H
#define URLSHORTNER_THREADPOOL_H

#include <queue>
#include <thread>
#include "ThreadPool.h"
#include "WorkerBase.h"


template<typename Work, class Worker>
class ThreadPool {
public:
    ThreadPool(int poolSize, int queueSize);

    void finish();

    bool addJob(Work *job);

    std::queue<Work *> jobs;
    std::condition_variable workToBeDone;
private:
    Work *getJob();

    std::mutex workMutex;
    std::mutex queue_mutex;
    std::vector<Worker *> workers;

    int poolSize;
    int queueSize;

};

template<typename Work, class Worker>
ThreadPool<Work, Worker>::ThreadPool(int poolSize, int queueSize) {
    this->poolSize = poolSize;
    if (queueSize < 1) {
        std::cout << "Invalid queue size. Fallback to 1." << std::endl;
        this->queueSize = 1;
    } else {
        this->queueSize = queueSize;
    }



    for (int i = 0; i < this->poolSize; ++i) {
        std::function<Work *(void)> gJ = std::bind(&ThreadPool::getJob, this);
        Worker *worker = new Worker(gJ,
                                    workMutex,
                                    this->workToBeDone);
        workers.push_back(worker);
    }

}

template<typename Work, class Worker>
Work *ThreadPool<Work, Worker>::getJob() {
    std::lock_guard<std::mutex> lock(queue_mutex);

    Work *job = NULL;

    if (!jobs.empty()) {
        job = jobs.front();
        jobs.pop();
        workToBeDone.notify_one();
    }

    return job;
}

template<typename Work, class Worker>
bool ThreadPool<Work, Worker>::addJob(Work *job) {
    bool result = false;

    if (jobs.size() < queueSize) {
        jobs.push(job);
        result = true;
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            workToBeDone.notify_all();
        }
    }

    return result;
}

template<typename Work, class Worker>
void ThreadPool<Work, Worker>::finish() {

    workToBeDone.notify_all();

    for (int i = 0; i < poolSize; ++i) {
        workers[i]->finish();
    }

    workToBeDone.notify_all();

}


#endif //URLSHORTNER_WORKERQUEUE_H
