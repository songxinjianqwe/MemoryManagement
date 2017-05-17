//
// Created by songx on 2017/5/17.
//

#ifndef MEMORYMANAGEMENT_PROCESS_H
#define MEMORYMANAGEMENT_PROCESS_H
struct PCB ** loadProcessTable();
void initProcessTable();
void createProcess(int pid);
void finalizeProcess(int pid);



#endif //MEMORYMANAGEMENT_PROCESS_H
