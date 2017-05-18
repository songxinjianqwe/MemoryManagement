//
// Created by songx on 2017/5/17.
//

#include "process.h"
#include <stdio.h>
#include "page_table.h"

struct PCBTable loadPCBTable() {
    struct PCBTable table;
    data_unit *ptr;
    for (unsigned i = 0; i < PROCESS_NUM; ++i) {
        //ptr指向每个PCB的开始
        ptr = (data_unit *) &table.pcbs[i];
        //读入一个PCB
        for (unsigned j = 0; j < PCB_SIZE; ++j) {
            *(ptr + j) = mem_read(PAGE_BIT_STRUCT_SIZE + PAGE_TABLE_SIZE + i * PCB_SIZE + j);
        }
    }
    return table;
}

void flushPCBTable(struct PCBTable table) {
    data_unit *ptr;
    for (unsigned i = 0; i < PROCESS_NUM; ++i) {
        //ptr指向每个PCB的开始
        ptr = (data_unit *) &table.pcbs[i];
        //读入一个PCB
        for (unsigned j = 0; j < PCB_SIZE; ++j) {
            mem_write(*(ptr + j), PAGE_BIT_STRUCT_SIZE + PAGE_TABLE_SIZE + i * PCB_SIZE + j);
        }
    }
}

void initPCBTable() {
    struct PCBTable table = loadPCBTable();
    for (unsigned i = 0; i < PROCESS_NUM; ++i) {
        table.pcbs[i].pid = 0;
    }
    flushPCBTable(table);
}

int createProcess(int pid, m_size_t size) {
    struct PCBTable table = loadPCBTable();
    if(table.pcbs[pid].pid != 0){
        return PID_DUPLICATED;
    }
    u2 occupiedPageSize;
    if (size % PAGE_FRAME_SIZE == 0) {
        occupiedPageSize = size / PAGE_FRAME_SIZE;
    } else {
        occupiedPageSize = size / PAGE_FRAME_SIZE + 1;
    }
    printf("%d %% PAGE_FRAME_SIZE %d\n",size,size % PAGE_FRAME_SIZE);
    printf("occupiedPageSize:%d\n",occupiedPageSize);
    //分配页框
    int result = allocatePageFrames(occupiedPageSize);
    if(result < 0){
        return result;
    }else{
        //分配进程号和页框个数
        table.pcbs[pid].pid = pid;
        table.pcbs[pid].pageSize = occupiedPageSize;
        table.pcbs[pid].pageTableStart = result;
        flushPCBTable(table);
        return SUCCESS;
    }
}

void finalizeProcess(int pid) {
    
}
