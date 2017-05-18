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
#include "address.h"
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
    struct PCB pcb = loadPCB(pid);
    if (pcb.pid == 0) {
        return PID_NOT_FOUND;
    }
    int pageNum = parseToStartAddress(address);
    int offset = parseToOffset(address);
    if (pageNum > pcb.pageSize) {
        return ACCESS_FAIL;
    }
//    printf("pageTableStart:%d\n",pcb.pageTableStart);
//    printf("pageNum:%d\n",pageNum);
    *data = readPage(pcb.pageTableStart + pageNum, offset);
    return SUCCESS;
}

int write(data_unit data, v_address address, m_pid_t pid) {
    struct PCB pcb = loadPCB(pid);
    if (pcb.pid == 0) {
        return PID_NOT_FOUND;
    }
    int pageNum = parseToStartAddress(address);
    int offset = parseToOffset(address);
    if (pageNum > pcb.pageSize) {
        return ACCESS_FAIL;
    }
    writePage(data,pcb.pageTableStart+pageNum,offset);
    return SUCCESS;
}


int allocate(v_address *address, m_size_t size, m_pid_t pid) {
    if (!isAllocatable(size)) {
        return OUT_OF_MEMORY;
    }
    int result = createProcess(pid, size);
    if (result >= 0) {
        *address = 0;
    }
    return result;
}

int free(v_address address, m_pid_t pid){
    printf("call free\n");
//     struct PCB pcb = loadPCB(pid);
//    if (pcb.pid == 0) {
//        return PID_NOT_FOUND;
//    }
//    int pageNum = parseToStartAddress(address);
//    int offset = parseToOffset(address);
//    if (pageNum > pcb.pageSize) {
//        return ACCESS_FAIL;
//    }
    
    return 0;
}