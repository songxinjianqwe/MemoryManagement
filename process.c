//
// Created by songx on 2017/5/17.
//

#include "process.h"
#include <stdio.h>

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

void createProcess(int pid, m_size_t size) {
    struct PCBTable table = loadPCBTable();
    for (unsigned i = 0; i < PROCESS_NUM; ++i) {
        if (table.pcbs[i].pid == 0) {
            table.pcbs[i].pid = pid;
            if (size % PAGE_FRAME_SIZE == 0) {
                table.pcbs[i].pageSize = size / PAGE_FRAME_SIZE;
            } else {
                table.pcbs[i].pageSize = size / PAGE_FRAME_SIZE + 1;
            }
            break;
        }
    }
}

void finalizeProcess(int pid) {

}
