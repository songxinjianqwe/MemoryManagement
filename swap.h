//
// Created by songx on 2017/5/19.
//

#ifndef MEMORYMANAGEMENT_SWAP_H
#define MEMORYMANAGEMENT_SWAP_H

#include "struct.h"

unsigned pageFaultInterrupt(unsigned diskAddress, m_pid_t pid);

int allocateVirtualPage();

void swapOut(unsigned diskAddress, unsigned pageFrameNum);
void swapIn(unsigned diskAddress, unsigned pageFrameNum);

void freeVirtualPage(unsigned pageNum);

void initSwap();

struct SwapBitMap loadSwapBitMap();

void flushSwapBitMap(struct SwapBitMap bitMap);


#endif //MEMORYMANAGEMENT_SWAP_H
