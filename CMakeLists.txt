cmake_minimum_required(VERSION 3.25)

project(llvm-frontend)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

set(SOURCES src/compiler.cpp)
add_executable(compiler ${SOURCES})
