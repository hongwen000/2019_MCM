cmake_minimum_required(VERSION 3.10)
project(BYL)

set(CMAKE_CXX_STANDARD 17)

add_executable(BYL main.cpp)
find_package(fmt)
target_link_libraries(BYL fmt)