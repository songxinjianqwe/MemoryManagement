cmake_minimum_required(VERSION 3.7)
project(MemoryManagement)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES bottom.c bottom.h call.h test.c type.h struct.h call.c)
add_executable(MemoryManagement ${SOURCE_FILES})