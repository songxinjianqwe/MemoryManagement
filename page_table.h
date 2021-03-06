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
void flushPage(unsigned  pageNum,struct PageItem page);

/**
 * 从指定物理地址读取一个字节
 * @param pageNum 
 * @param offset 
 * @param pid 
 * @return 
 */
data_unit readPage(unsigned pageNum, unsigned offset,m_pid_t pid);

int writePage(data_unit data,unsigned pageNum,unsigned offset,m_pid_t pid);
bool isAccessFail(struct PCB pcb, v_address address);
/**
 * 改进的时钟页面替换算法
 * 同时考虑引用位和修改位
 * @param pid 
 * @return 
 */
unsigned clockPaging(m_pid_t pid);
/**
 * 全部分配物理页框
 * @param pageSize 
 * @param pageTable 
 * @param pageStart 
 * @return 
 */
int allocateAllInMemory(unsigned pageSize, struct PageTable pageTable, unsigned pageStart);
/**
 * 仅为第一页分配物理页框
 * @param pageSize 
 * @param pageTable 
 * @param pageStart 
 * @return 
 */
int allocateWithSwap(unsigned pageSize, struct PageTable pageTable, unsigned pageStart);


bool isPageUsed(u2 sign);

bool isPageInMainMemory(u2 sign);
bool isPageReferred(u2 sign);

bool isPageModified(u2 sign);


#endif //MEMORYMANAGEMENT_PAGE_TABLE_H
