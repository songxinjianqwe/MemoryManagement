cmake_minimum_required(VERSION 3.7)
project(MemoryManagement)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES bottom.c bottom.h call.h test.c type.h struct.h call.c initialize.c initialize.h process.h process.c page_frame.h page_frame.c page_table.h page_table.c)
add_executable(MemoryManagement ${SOURCE_FILES})