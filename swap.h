//
// Created by songx on 2017/5/19.
//

#ifndef MEMORYMANAGEMENT_SWAP_H
#define MEMORYMANAGEMENT_SWAP_H
#include "struct.h"

struct PageItem  pageFaultInterrupt(struct PageItem page);
void initSwap();
int allocateOneDiskPage(unsigned pageNum);
void freeSwap();

#endif //MEMORYMANAGEMENT_SWAP_H
