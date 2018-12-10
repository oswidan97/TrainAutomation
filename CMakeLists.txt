cmake_minimum_required(VERSION 3.7)
project(MutualExclusionAndSynchronization)

set(CMAKE_C_STANDARD 11)
set(THREADS_PREFER_PTHREAD-FLAG ON)
find_package(Threads REQUIRED)

set(SOURCE_FILES main.c train.c train.h)
add_executable(MutualExclusionAndSynchronization ${SOURCE_FILES})
target_link_libraries(MutualExclusionAndSynchronization ${CMAKE_THREAD_LIBS_INIT})