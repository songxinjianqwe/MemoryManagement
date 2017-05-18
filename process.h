//
// Created by songx on 2017/5/17.
//

#ifndef MEMORYMANAGEMENT_PROCESS_H
#define MEMORYMANAGEMENT_PROCESS_H
#include "struct.h"

struct PCBTable loadPCBTable();
void initPCBTable();
int createProcess(int pid,m_size_t size);
void finalizeProcess(int pid);
void flushPCBTable(struct PCBTable table);


#endif //MEMORYMANAGEMENT_PROCESS_H
