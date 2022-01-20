cmake_minimum_required (VERSION 3.8)

project(clw-08)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

# Uncomment to work on task 1, fib
# add_executable(fibonacci_test src/task_1/test.cpp)

# Uncomment to work on task 2, functions
#add_executable(functions_test src/task_2/test.cpp)

# Uncomment to work on task 3, queue
# add_executable(queue_test src/task_3/test.cpp)

# Uncomment to work on task 4, priority_queue
 add_executable(priority_queue_test src/task_4/test.cpp)

