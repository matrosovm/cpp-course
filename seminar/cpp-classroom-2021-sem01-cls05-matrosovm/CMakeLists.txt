cmake_minimum_required (VERSION 3.8)

project(clw-04)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

#Uncomment to work on task 1, shared_buffer
#add_executable(shared_buffer_test src/task_1/shared_buffer.cpp src/task_1/test_shared_buffer.cpp)

# Uncomment to work on task 1, lazy_string
#add_executable(lazy_string_test src/task_1/shared_buffer.cpp src/task_1/lazy_string.cpp src/task_1/test_lazy_string.cpp)

# Uncomment to work on task 1, lazy_string_access_op
#add_executable(lazy_string_access_op_test src/task_1/shared_buffer.cpp src/task_1/lazy_string.cpp src/task_1/test_access_operator.cpp)

# Uncomment to work on task 2
add_executable(read_write_test src/task_2/read_write.cpp src/task_2/test.cpp)
