cmake_minimum_required(VERSION 3.9)
project(hw3_prob3)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(hw3_prob3 ${SOURCE_FILES})
target_link_libraries(hw3_prob3 ${OpenCV_LIBS})
