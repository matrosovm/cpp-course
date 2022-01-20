cmake_minimum_required (VERSION 3.8)

project(clw-06)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

include_directories(src)

# add_executable(task-1-test src/tests/task_1_test.cpp src/my_ostream.cpp src/my_ostream_con.cpp)
# add_executable(task-2-test src/tests/task_2_test.cpp src/my_ostream.cpp src/my_ostream_file.cpp)
# add_executable(task-3-test src/tests/task_3_test.cpp src/my_ostream.cpp src/my_ostream_con.cpp src/my_ostream_file.cpp src/my_ostream_combo.cpp)
add_executable(task-4-test src/tests/task_4_test.cpp src/get_size.cpp)
