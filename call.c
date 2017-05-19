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
#include "swap.h"
#include "external_page_table.h"

#include <stdio.h>

/**
 * 打印初始化参数
 */
void printInitParams() {
    printf("DISK_SIZE:%d\n",DISK_SIZE);
    printf("MEMORY_SIZE:%d\n", MEMORY_SIZE);
    printf("PAGE_FRAME_SIZE:%d\n", PAGE_FRAME_SIZE);
    printf("TOTAL_PAGE_FRAME_NUM:%d\n", TOTAL_PAGE_FRAME_NUM);
    printf("TOTAL_PAGE_NUM:%d\n", TOTAL_PAGE_NUM);
    printf("PAGE_BIT_MAP_SIZE:%d\n", PAGE_BIT_MAP_SIZE);
    printf("PAGE_BIT_STRUCT_SIZE:%d\n", PAGE_BIT_STRUCT_SIZE);
    printf("PAGE_TABLE_ITEM_SIZE:%d\n", PAGE_TABLE_ITEM_SIZE);
    printf("PAGE_TABLE_SIZE:%d\n", PAGE_TABLE_SIZE);
    printf("PROCESS_NUM:%d\n", PROCESS_NUM);
    printf("PCB_SIZE:%d\n", PCB_SIZE);
    printf("PCB_TABLE_SIZE:%d\n", PCB_TABLE_SIZE);
    printf("SWAP_BIT_MAP_SIZE:%d\n", SWAP_BIT_MAP_SIZE);
    printf("SWAP_BIT_STRUCT_SIZE:%d\n", SWAP_BIT_STRUCT_SIZE);
    printf("PAGE_FRAME_BEGIN_POS:%d\n", PAGE_FRAME_BEGIN_POS);
}


void init() {
    initPageBitMap();
    initPageTable();
    initExternalPageTable();
    initPCBTable();
    initSwap();
}

int read(data_unit *data, v_address address, m_pid_t pid) {
    struct PCB pcb = loadPCB(pid);
    if (pcb.pid == 0) {
        return PID_NOT_FOUND;
    }
    int pageNum = parseToStartAddress(address);
    int offset = parseToOffset(address);
    if (isAccessFail(pcb, address)) {
        return ACCESS_FAIL;
    }
    *data = readPage(pcb.pageTableStart + pageNum, offset,pid);
    return SUCCESS;
}

int write(data_unit data, v_address address, m_pid_t pid) {
    struct PCB pcb = loadPCB(pid);
    if (pcb.pid == 0) {
        return PID_NOT_FOUND;
    }
    int pageNum = parseToStartAddress(address);
    int offset = parseToOffset(address);
    if (isAccessFail(pcb, address)) {
        return ACCESS_FAIL;
    }
    writePage(data, pcb.pageTableStart + pageNum, offset,pid);
    return SUCCESS;
}

int allocate(v_address *address, m_size_t size, m_pid_t pid) {
    if (!isAllocatable()) {
        //只要有一个空闲页框，就可以创建进程
        return OUT_OF_MEMORY;
    }
    int result = createProcess(pid, size);
    if (result >= 0) {
        *address = 0;
    }
    return result;
}

int free_memory(v_address address, m_pid_t pid) {
    struct PCB pcb = loadPCB(pid);
    if (pcb.pid == 0) {
        return PID_NOT_FOUND;
    }
    if (isAccessFail(pcb, address)) {
        return ACCESS_FAIL;
    }
    finalizeProcess(pcb);
    return SUCCESS;
}