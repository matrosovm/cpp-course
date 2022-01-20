cmake_minimum_required(VERSION 3.16)
project(cls_08)

set(CMAKE_CXX_STANDARD 20)

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

enable_testing()


add_executable(test_11 src/task_1/test_11.cpp src/task_1/pretty_print.h)
target_link_libraries(test_11 gtest_main)

add_executable(test_12 src/task_1/test_12.cpp src/task_1/pretty_print.h)
target_link_libraries(test_12 gtest_main)

add_executable(test_13 src/task_1/test_13.cpp src/task_1/pretty_print.h)
target_link_libraries(test_13 gtest_main)

add_executable(test_14 src/task_1/test_14.cpp src/task_1/pretty_print.h)
target_link_libraries(test_14 gtest_main)
