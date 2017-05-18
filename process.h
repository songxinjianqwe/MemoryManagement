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
int createProcess(m_pid_t pid,m_size_t size);
void finalizeProcess(struct PCB pcb);
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
