cmake_minimum_required (VERSION 3.8)

project(semester-02-control-03)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

include_directories(src)

# add_executable(control-3-task-1-test src/task_1/smoke.cpp)
# add_executable(control-3-task-2-test src/task_2/smoke.cpp)
