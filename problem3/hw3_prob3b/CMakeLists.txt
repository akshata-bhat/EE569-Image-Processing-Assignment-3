# Author : Akshata Bhat
# USC ID : 9560895350
# Email : akshatab@usc.edu
# Submission Date : 3/28/2018

cmake_minimum_required(VERSION 3.9)
project(hw3_prob3b)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(hw3_prob3b ${SOURCE_FILES})
target_link_libraries(hw3_prob3b ${OpenCV_LIBS})
