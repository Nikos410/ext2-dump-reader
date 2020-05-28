cmake_minimum_required(VERSION 3.16)
project(ext2_utils)

set(CMAKE_CXX_STANDARD 20)

add_executable(ext2_utils main.cpp Ext2DumpReader.cpp Ext2DumpReader.hpp Ext2SuperBlockHelper.cpp Ext2SuperBlockHelper.hpp)
