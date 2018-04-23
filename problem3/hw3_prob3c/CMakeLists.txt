cmake_minimum_required(VERSION 3.9)
project(BagOfWords)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(BagOfWords ${SOURCE_FILES})
target_link_libraries(BagOfWords ${OpenCV_LIBS})
