//
// Created by songx on 2017/5/16.
//

#include "type.h"
#include "struct.h"
#include "call.h"
#include "bottom.h"
#include "process.h"
#include "page_table.h"
#include "page_bit_map.h"

#include <stdio.h>

/**
 * 打印初始化参数
 */
void printInitParams() {
    printf("MEMORY_SIZE:%d\n", MEMORY_SIZE);
    printf("PAGE_FRAME_SIZE:%d\n", PAGE_FRAME_SIZE);
    printf("TOTAL_PAGE_NUM:%d\n", TOTAL_PAGE_NUM);
    printf("PAGE_BIT_MAP_SIZE:%d\n", PAGE_BIT_MAP_SIZE);
    printf("PAGE_BIT_STRUCT_SIZE:%d\n", PAGE_BIT_STRUCT_SIZE);
    printf("PAGE_TABLE_ITEM_SIZE:%d\n", PAGE_TABLE_ITEM_SIZE);
    printf("PAGE_TABLE_SIZE:%d\n", PAGE_TABLE_SIZE);
    printf("PROCESS_NUM:%d\n", PROCESS_NUM);
    printf("PCB_SIZE:%d\n", PCB_SIZE);
    printf("PCB_TABLE_SIZE:%d\n", PCB_TABLE_SIZE);
    printf("PAGE_FRAME_BEGIN_POS:%d\n", PAGE_FRAME_BEGIN_POS);
}

void init() {
    initPageBitMap();
    initPageTable();
    initPCBTable();
}

int read(data_unit *data, v_address address, m_pid_t pid) {
    return 0;
}

int write(data_unit data, v_address address, m_pid_t pid) {
    return 0;
}

int allocate(v_address *address, m_size_t size, m_pid_t pid) {
    if (!isAllocatable(size)) {
        return ERR_OUT_OF_BOUNDS;
    }
    createProcess(pid, size);
    return 0;
}

int free(v_address address, m_pid_t pid) {
    return 0;
}