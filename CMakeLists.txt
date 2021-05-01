cmake_minimum_required(VERSION 3.16.3)

project(filosofos-comensales)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/utils/*.cpp)

file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/src/headers/*.hpp)

add_executable(${PROJECT_NAME} src/main.cpp ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${PROJECT_SOURCE_DIR}/src/headers)

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
