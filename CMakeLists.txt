cmake_minimum_required(VERSION 3.6)
project(msl_clang_001)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES BinTreeWords.c)

add_executable(BinTreeWords ${SOURCE_FILES})
