cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

project(shmem)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(shmem STATIC src/Semaphore.cpp src/SharedMemory.cpp src/Exceptions.cpp)

target_include_directories(shmem PUBLIC include/)

add_subdirectory(example)