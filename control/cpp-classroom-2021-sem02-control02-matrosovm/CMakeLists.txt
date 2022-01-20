cmake_minimum_required (VERSION 3.8)

project(control-02)

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

add_executable(task-1-test src/task_1/buffer_smoke_test.cpp src/task_1/buffer.hpp)
target_link_libraries(task-1-test gtest_main)



if (WIN32)
    set(dl_loader_platform windows)
endif(WIN32)

if(UNIX)
    set(dl_loader_platform unix)
endif(UNIX)

set(DLLOADER_INC_PATH src/task_2/${dl_loader_platform})
set(DLLOADER_SRC src/task_2/${dl_loader_platform}/dlloader.hpp)

add_executable(task-2-test src/task_2/phonebook_smoke_test.cpp src/task_2/phonebook.hpp)
target_link_libraries(task-2-test gtest_main)
add_compile_definitions(PARSER_LIBRARY_DIR="${CMAKE_BINARY_DIR}")
target_include_directories(task-2-test PRIVATE ${DLLOADER_INC_PATH} src/task_2)