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

struct PCB loadPCB(m_pid_t pid) {
    struct PCB pcb;
    data_unit *ptr = (data_unit *) &pcb;
    //读入一个PCB
    for (unsigned i = 0; i < PCB_SIZE; ++i) {
        *(ptr + i) = mem_read(PAGE_BIT_STRUCT_SIZE + PAGE_TABLE_SIZE + pid * PCB_SIZE + i);
    }
    return pcb;
}

void flushPCB(struct PCB pcb) {
    data_unit *ptr = (data_unit *) &pcb;
    for (unsigned i = 0; i < PCB_SIZE; ++i) {
        mem_write(*(ptr + i), PAGE_BIT_STRUCT_SIZE + PAGE_TABLE_SIZE + pcb.pid * PCB_SIZE + i);
    }
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
        table.pcbs[i].pageTableStart = 0;
        table.pcbs[i].pageSize = 0;
        table.pcbs[i].lastPageLimit = 0;
    }
    flushPCBTable(table);
}

int createProcess(m_pid_t pid, m_size_t size) {
    struct PCB pcb = loadPCB(pid);
    if (pcb.pid != 0) {
        return PID_DUPLICATED;
    }
    u2 occupiedPageSize;
    if (size % PAGE_FRAME_SIZE == 0) {
        occupiedPageSize = size / PAGE_FRAME_SIZE;
    } else {
        occupiedPageSize = size / PAGE_FRAME_SIZE + 1;
    }
//    printf("%d %% PAGE_FRAME_SIZE %d\n", size, size % PAGE_FRAME_SIZE);
//    printf("occupiedPageSize:%d\n", occupiedPageSize);
    
    //分配页框
    int result = allocatePageFrames(occupiedPageSize);
    if (result < 0) {
        return result;
    } else {
        //分配进程号和页框个数
        pcb.pid = pid;
        pcb.pageSize = occupiedPageSize;
        pcb.pageTableStart = result;
        pcb.lastPageLimit = size % PAGE_FRAME_SIZE;
        flushPCB(pcb);
        return SUCCESS;
    }
}

void finalizeProcess(struct PCB pcb) {
    freePageFrames(pcb.pageTableStart, pcb.pageSize);
    //将pcb清空
    for (unsigned i = 0; i < PCB_SIZE; ++i) {
        mem_write(0, PAGE_BIT_STRUCT_SIZE + PAGE_TABLE_SIZE + pcb.pid * PCB_SIZE + i);
    }
}
