cmake_minimum_required(VERSION 3.8)
project(semester_02_cls_02)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS   "-Wall -Wextra -Werror")

#add_executable(task_1_smoke_test task1/smoke_test.cpp task1/typeset.hpp)
add_executable(task_2_smoke_test task2/smoke_test.cpp task2/pretty_print.hpp)
#add_executable(task3_smoke_test task3/smoke_test.cpp task3/projections.hpp)
