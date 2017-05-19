//
// Created by songx on 2017/5/19.
//

#ifndef MEMORYMANAGEMENT_SWAP_H
#define MEMORYMANAGEMENT_SWAP_H

#include "struct.h"

unsigned pageFaultInterrupt(unsigned diskPageNum, m_pid_t pid);

int allocateVirtualPage();
/**
 * 将一页的数据从页框拷贝到磁盘中
 * @param diskPageNum 
 * @param pageFrameNum 
 */
void swapOut(unsigned diskPageNum, unsigned pageFrameNum);
/**
 * 将一页的数据从磁盘拷贝到页框中
 * @param diskPageNum 
 * @param pageFrameNum 
 */
void swapIn(unsigned diskPageNum, unsigned pageFrameNum);

void freeVirtualPage(unsigned pageNum);

void initSwap();

struct SwapBitMap loadSwapBitMap();

void flushSwapBitMap(struct SwapBitMap bitMap);


#endif //MEMORYMANAGEMENT_SWAP_H
