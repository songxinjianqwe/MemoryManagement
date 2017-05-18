//
// Created by songx on 2017/5/17.
//

#ifndef MEMORYMANAGEMENT_PAGE_TABLE_H
#define MEMORYMANAGEMENT_PAGE_TABLE_H
#include "type.h"
#include "struct.h"

struct PageTable loadPageTable();
void flushPageTable(struct PageTable table);
void initPageTable();
/**
 * 分配pageSize个页框，返回在页表中的起址
 * @param pageSize 
 * @return 
 */
int allocatePageFrames(unsigned pageSize);


#endif //MEMORYMANAGEMENT_PAGE_TABLE_H
