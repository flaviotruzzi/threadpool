cmake_minimum_required(VERSION 3.7)
project(threadpool)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp WorkerBase.h ThreadPool.h)
add_executable(threadpool ${SOURCE_FILES})