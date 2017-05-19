//
// Created by songx on 2017/5/17.
//

#ifndef MEMORYMANAGEMENT_PROCESS_H
#define MEMORYMANAGEMENT_PROCESS_H

#include "struct.h"

/**
 * 加载进程表
 * @return  PCBTable
 */
struct PCBTable loadPCBTable();

/**
 * 初始化进程表
 */
void initPCBTable();

/**
 * 创建进程，并为其分配内存
 * @param pid 
 * @param size 
 * @return 
 */
int createProcess(m_pid_t pid, m_size_t size);

/**
 * 销毁进程，并释放其内存和交换区
 * @param pcb 
 */
void finalizeProcess(struct PCB pcb);

/**
 * 写回进程表
 * @param table 
 */
void flushPCBTable(struct PCBTable table);

/**
 * 加载某个进程的PCB
 * @param pid 
 * @return 
 */
struct PCB loadPCB(m_pid_t pid);

/**
 * 写回某个进程的PCB
 * @param pcb 
 */
void flushPCB(struct PCB pcb);

#endif //MEMORYMANAGEMENT_PROCESS_H
