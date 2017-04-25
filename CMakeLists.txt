cmake_minimum_required(VERSION 3.7)
project(urlshortner)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp Worker.cpp Worker.h WorkerQueue.cpp WorkerQueue.h)
add_executable(urlshortner ${SOURCE_FILES})