//
// Created by songx on 2017/5/17.
//

#ifndef MEMORYMANAGEMENT_PAGE_BIT_MAP_H
#define MEMORYMANAGEMENT_PAGE_BIT_MAP_H
#include "struct.h"
#include "stdbool.h"
/**
 * 读取PageBitMap，只可以读，不可以写
 * @return  pageBitMap
 */
struct PageBitMap  loadPageBitMap();


/**
 * 将修改后的pageBitMap写回到内存
 * @param bitMap 
 */
void flushPageBitMap(struct PageBitMap bitMap);


/**
 * 初始化位示图
 */
void initPageBitMap();

bool isAllocatable();

/**
 * 分配一个页框，修改位示图和空闲块数，返回页框号
 * @return 
 */
int allocatePhysicalPage();
void freePhysicalPage(unsigned pageFrameNum);

#endif //MEMORYMANAGEMENT_PAGE_BIT_MAP_H
