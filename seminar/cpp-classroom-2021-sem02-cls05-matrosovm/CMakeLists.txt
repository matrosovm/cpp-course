cmake_minimum_required(VERSION 3.12)
project(libs)

set(CMAKE_CXX_STANDARD 20)

file(GLOB TASK_1
        src/task_1/*.hpp
        src/task_1/*.cpp)

file(GLOB TASK_2
        src/task_2/*.hpp
        src/task_2/*.cpp)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(task_1 ${TASK_1})
add_executable(task_2 ${TASK_2})

target_link_libraries(task_1 ${GTEST_LIBRARIES} pthread boost_serialization)
target_link_libraries(task_2 ${GTEST_LIBRARIES} pthread)