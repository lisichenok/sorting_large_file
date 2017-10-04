cmake_minimum_required(VERSION 3.8)
project(sorting_large_file)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp buffer/FileBufferBase.h buffer/FileReadBuffer.h buffer/FileWriteBuffer.h helper/FileHelper.h FileSorter.h helper/NumberHelper.h Segment.h helper/FileSorterHelper.h )
add_executable(sorting_large_file ${SOURCE_FILES} buffer/FileBufferBase.h buffer/FileReadBuffer.h buffer/FileWriteBuffer.h helper/FileHelper.h FileSorter.h helper/NumberHelper.h Segment.h helper/FileSorterHelper.h )