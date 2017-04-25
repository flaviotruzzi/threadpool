#include <iostream>
#include <chrono>
#include "WorkerQueue.h"

static std::mutex mtx;
using namespace std::chrono_literals;

void printInt(int *a) {
    std::this_thread::sleep_for(200us);
    std::cout << "Thread: " << pthread_self() << " got " << *a << std::endl;
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    WorkerQueue<int> workerQueue(5, 5000, &printInt);

    int X = 0;
    int i = 0;
    while (i < 10000) {
        int *k = new int(i);
        if (!workerQueue.addJob(k)) {
            X = X + 1;
        }
        i++;
        std::this_thread::sleep_for(10us);
    }

    std::cout << "SEND ALL" << std::endl;

    workerQueue.finish();

    std::cout << std::endl << X << std::endl;

    return 0;
}