cmake_minimum_required (VERSION 3.8)

project(clw-10)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

#add_executable(task-1-test src/task_1/test.cpp)
#add_executable(task-2-test src/task_2/test.cpp)
add_executable(task-3-test src/task_3/test.cpp)
#add_executable(task-4-test src/task_4/smoke_test.cpp)