cmake_minimum_required (VERSION 3.8)

project(clw-04)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

# Uncomment to work on task 1
#add_executable(compose_test src/task_1/composition.cpp src/task_1/test.cpp)

# Uncomment to work on task 2
# add_executable(sum_test src/task_2/sum.c src/task_2/test.cpp)

# Uncomment to work on task 3
#add_executable(enum_test src/task_3/test.cpp src/task_3/enum.cpp)

# Uncomment to work on task 4
add_executable(string_view_test src/task_4/string_view.cpp src/task_4/test.cpp)
