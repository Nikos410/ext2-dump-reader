cmake_minimum_required(VERSION 3.16)
project(ext2_utils)

set(CMAKE_CXX_STANDARD 20)

add_executable(ext2_utils main.cpp DumpReader.cpp DumpReader.hpp BlockHelper.cpp BlockHelper.hpp InodeHelper.cpp InodeHelper.hpp)
