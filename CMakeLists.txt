cmake_minimum_required(VERSION 3.9)
project(splc)

set(CMAKE_CXX_STANDARD 11)
set(INC_DIR /usr/local/opt/llvm/include)
set(LINK_DIR /usr/local/opt/llvm/lib)
include_directories(${INC_DIR})
link_directories(${LINK_DIR})

add_executable(splc main.cpp node.h)