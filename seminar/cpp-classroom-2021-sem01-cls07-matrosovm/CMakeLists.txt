cmake_minimum_required (VERSION 3.8)

project(clw-07)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

# Uncomment to work on task 1, json_value
# add_executable(json_value_test src/task_1/json_value.cpp src/task_1/test_json_value.cpp)

# Uncomment to work on task 2, leak_detector
# add_executable(leak_detector_test src/task_2/leak_detector.cpp src/task_2/test_leak_detector.cpp)
