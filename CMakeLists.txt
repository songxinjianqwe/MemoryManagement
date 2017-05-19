cmake_minimum_required(VERSION 3.7)
project(MemoryManagement)

set(CMAKE_C_STANDARD 99)
set(SOURCE_FILES bottom.c bottom.h call.h test.c type.h struct.h call.c process.h process.c page_table.h page_table.c main.c page_bit_map.h page_bit_map.c address.h address.c swap.h swap.c external_page_table.h external_page_table.c)
add_executable(MemoryManagement ${SOURCE_FILES} )
MATH(EXPR stack_size "8 * 1024 * 1024") # 16 Mb
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")