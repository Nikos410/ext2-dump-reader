cmake_minimum_required(VERSION 3.16)
project(simple-ext2-reader)

set(CMAKE_CXX_STANDARD 20)

add_executable(simple-ext2-reader main.cpp DumpReader.cpp DumpReader.hpp BlockHelper.cpp BlockHelper.hpp InodeHelper.cpp InodeHelper.hpp)
