# threadpool

A C++ study implementation of a Queued back ThreadPool.

`ThreadPool` accepts work and insert it into its queue. As soon it notices
work, it wake up the workers and they work until the queue is completely consumed
going back to sleep afterwards. If the workers can't handle the load, `ThreadPool`
will start to reject requests.

The user needs to extend the `WorkerBase` class, and implement the method
`work`.

Usage (extracted from `main.cpp`):

```C++
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
```

To build and run the example:

```sh
➜  threadpool git:(master) ✗ mkdir build
➜  threadpool git:(master) ✗ cd build
➜  build git:(master) ✗  cmake ../
➜  build git:(master) ✗ make
Scanning dependencies of target threadpool
[ 50%] Building CXX object CMakeFiles/threadpool.dir/main.cpp.o
[100%] Linking CXX executable threadpool
[100%] Built target threadpool

➜  build git:(master) ✗ ./threadpool
usage: ./threadpool threads queue_size tasks

➜  build git:(master) ✗ ./threadpool 5 10 100
Initializing ThreadPool with 5 threads and queue size of 10
Rejected tasks: 8
```