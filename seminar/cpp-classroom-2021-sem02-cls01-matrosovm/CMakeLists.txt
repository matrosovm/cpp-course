cmake_minimum_required (VERSION 3.8)

project(clw-01)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ") #-Wall -Werror -Wextra")
endif()

#add_executable(task-1-test src/task_1/test.cpp src/task_1/split.hpp)
#add_executable(task-2-test src/task_2/test.cpp src/task_2/permutations.hpp)
#add_executable(task-3-test src/task_3/test.cpp src/task_3/tricky_shared_ptr.hpp)
add_executable(task-4-test src/task_4/test.cpp src/task_4/timer.hpp)
#add_executable(task-5-test src/task_5/test.cpp src/task_5/zip.hpp)
