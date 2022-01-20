cmake_minimum_required(VERSION 3.19)
project(template_repo)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wextra-semi \
                        -O0 -g -fsanitize=address -fno-omit-frame-pointer")
endif()

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

add_executable(task-1-test src/task_1/cowjson.hpp src/task_1/test.cpp)
target_link_libraries(task-1-test gtest_main)


add_library(calc_lib src/task_2/calculate_impl/CalcAlgorythm.c src/task_2/calculate_impl/CalcError.c src/task_2/calculate_impl/CalcInit.c src/task_2/calculate_impl/CalcInterface.c)
target_link_libraries(calc_lib PRIVATE m)

add_executable(calc src/task_2/calculate_impl/Main.c)
target_link_libraries(calc calc_lib)


add_library(calculate_lib_cpp src/task_2/calculate.hpp src/task_2/calculate.cpp)
target_link_libraries(calculate_lib_cpp calc_lib)

add_executable(task-2-calculate-test src/task_2/test_calculate.cpp)
target_link_libraries(task-2-calculate-test calculate_lib_cpp)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
add_executable(add-student src/in_class/add_student.cpp src/in_class/student.pb.cc)
target_link_libraries(add-student ${Protobuf_LIBRARIES})