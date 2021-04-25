cmake_minimum_required(VERSION 3.16.3)

project(filosofos-comensales)

include_directories(${PROJECT_SOURCE_DIR}/src/headers)

add_library(
    diningTable
    ${PROJECT_SOURCE_DIR}/src/utils/DiningTable.cpp
)

add_library(
    philosopher
    ${PROJECT_SOURCE_DIR}/src/utils/Philosopher.cpp
)

add_executable("${PROJECT_NAME}" src/main.c)

target_link_libraries(${PROJECT_NAME} diningTable philosopher )
