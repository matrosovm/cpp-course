cmake_minimum_required (VERSION 3.8)

project(control-01)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")
endif()

set(USE_GTEST 1)

if(USE_GTEST)
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

    add_executable(task-1-test-1 src/task_1/test_check_top_sp.cpp)
    add_executable(task-1-test-2 src/task_1/test_collapse.cpp)
    add_executable(task-1-test-3 src/task_1/test_top_by_project.cpp)

    add_executable(task-2-test-1 src/task_2/test_1.cpp src/task_2/maslenica.hpp)
    add_executable(task-2-test-2 src/task_2/test_2.cpp src/task_2/maslenica.hpp)

    add_executable(task-3-test src/task_3/test.cpp src/task_3/for_macro.hpp)
    target_link_libraries(task-3-test gtest_main)

else()
    add_executable(task-1-test-1 src/task_1/test_check_top_sp.cpp)
    add_executable(task-1-test-2 src/task_1/test_collapse.cpp)
    add_executable(task-1-test-3 src/task_1/test_top_by_project.cpp)

    add_executable(task-2-test-1 src/task_2/test_1.cpp src/task_2/maslenica.hpp)
    add_executable(task-2-test-2 src/task_2/test_2.cpp src/task_2/maslenica.hpp)

    add_executable(task-3-test src/task_3/test_no_gtest.cpp src/task_3/for_macro.hpp)
endif()

