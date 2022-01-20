cmake_minimum_required(VERSION 3.0)
project(cls_multithread)

set(CMAKE_CXX_STANDARD 20)

enable_testing()
#FIND_PACKAGE(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIR})

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wextra -Werror")
endif()

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DUSE_GTEST")

# see https://github.com/google/googletest/blob/master/googletest/README.md
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

#---------------

add_executable(task_1_test_parallel_for
        src/task_1/test_parallel_for.cpp
        src/task_1/parallel_for.hpp
)
target_link_libraries(task_1_test_parallel_for gtest_main)

add_executable(task_1_test_parallel_for_with_exceptions
        src/task_1/test_parallel_for_with_exceptions.cpp
        src/task_1/parallel_for.hpp
)
target_link_libraries(task_1_test_parallel_for_with_exceptions gtest)

add_executable(task_1_run_parallel_for
        src/task_1/run_parallel_for.cpp
        src/task_1/parallel_for.hpp
)

#----------------

add_executable(task_2_test_buffer
        src/task_2/test_buffer.cpp
        src/task_2/flushing_buffer.hpp
)
target_link_libraries(task_2_test_buffer gtest)

add_executable(task_2_test_buffer_hard_case
        src/task_2/test_buffer_hard_case.cpp
        src/task_2/flushing_buffer.hpp
        )
target_link_libraries(task_2_test_buffer_hard_case gtest)

#----------------

add_executable(task_3_test_semaphore
        src/task_3/test_semaphore.cpp
        src/task_3/semaphore.hpp
)
target_link_libraries(task_3_test_semaphore gtest_main)

add_executable(task_3_test_rwlock
        src/task_3/test_rwlock.cpp
        src/task_3/rwlock.hpp
)
target_link_libraries(task_3_test_rwlock gtest_main)

add_executable(task_3_test_queue
        src/task_3/test_queue.cpp
        src/task_3/queue.hpp
        )
target_link_libraries(task_3_test_queue gtest_main)
