cmake_minimum_required(VERSION 3.10.2)
project(ExampleProject)
add_library(app SHARED dummy.cpp)
target_link_libraries(app)
