cmake_minimum_required(VERSION 3.16.3)

project(filosofos-comensales)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/src/headers)

add_library(diningTable ${PROJECT_SOURCE_DIR}/src/utils/DiningTable.cpp)
target_link_libraries(diningTable PRIVATE Threads::Threads)

add_library(philosopher ${PROJECT_SOURCE_DIR}/src/utils/Philosopher.cpp)

add_executable("${PROJECT_NAME}" src/main.cpp)

target_link_libraries(${PROJECT_NAME} diningTable philosopher)
