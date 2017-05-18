//
// Created by songx on 2017/5/17.
//

#ifndef MEMORYMANAGEMENT_PAGE_TABLE_H
#define MEMORYMANAGEMENT_PAGE_TABLE_H
#include "type.h"
#include "struct.h"
#include "stdbool.h"

struct PageTable loadPageTable();
void flushPageTable(struct PageTable table);
void initPageTable();
/**
 * 分配pageSize个页框，返回在页表中的起址
 * @param pageSize 
 * @return 
 */
int allocatePageFrames(unsigned pageSize);

void freePageFrames(unsigned pageTableStart, unsigned pageSize);

struct PageItem loadPage(unsigned pageNum);
void flushPage(struct PageItem page);

/**
 * 给出页号和页内偏移，读取一个字节
 * @param pageNum 
 * @param offset 
 * @return 
 */
data_unit readPage(unsigned pageNum,unsigned offset);

int writePage(data_unit data,unsigned pageNum,unsigned offset);
bool isAccessFail(struct PCB pcb, v_address address);

#endif //MEMORYMANAGEMENT_PAGE_TABLE_H
