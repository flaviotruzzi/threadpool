#include <iostream>
#include <chrono>
#include "ThreadPool.h"
#include "WorkerBase.h"


using namespace std::chrono_literals;

class IntWorker : public WorkerBase<int> {
public:
    IntWorker(std::function<int *(void)> getWork_,
              std::mutex &mutex_,
              std::condition_variable &cv_)
            : WorkerBase(getWork_, mutex_, cv_) {}

private:
    void work(int *work) {
        std::this_thread::sleep_for(7ms);
    }
};


int main(int argc, char *argv[]) {

    if (argc != 4) {
        std::cout << "usage: " << argv[0] << " threads queue_size tasks" << std::endl;
        exit(-1);
    }

    int threadNum = std::stoi(argv[1]);
    int queueSize = std::stoi(argv[2]);
    int tasks = std::stoi(argv[3]);

    std::cout << "Initializing ThreadPool with queue size of "
              << queueSize << std::endl;
    ThreadPool<int, IntWorker> workerQueue(threadNum, queueSize);


    int rejectedTasks = 0;
    for (int i = 0; i < tasks; ++i) {
        int *k = new int(i);
        if (!workerQueue.addJob(k)) {
            rejectedTasks = rejectedTasks + 1;
        }
        std::this_thread::sleep_for(1ms);
    }

    workerQueue.finish();

    std::cout << "Rejected tasks: " << rejectedTasks << std::endl;

    return 0;
}