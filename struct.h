//
// Created by songx on 2017/5/16.
//

#ifndef MEMORYMANAGEMENT_STRUCT_H
#define MEMORYMANAGEMENT_STRUCT_H
#include "call.h"
#include "bottom.h"
#define PAGE_SIZE 1024*4

#define TOTAL_PAGE_NUM (MEMORY_SIZE/PAGE_SIZE) //2^15个页框

#define PAGE_BIT_MAP_SIZE (TOTAL_PAGE_NUM/1024)//2^12个字节



typedef unsigned int u4;
typedef unsigned short u2;
typedef unsigned char u1;

/**
 * 进程结构
 */
struct process{
    u2 pid;
    u4 pageTableStart;
    u2 pageFrameSize;
};

/**
 * 位示图管理页框的占用情况
 */
struct pageBitMap{
    u2 freePageFrame;
    u1 bits[PAGE_BIT_MAP_SIZE];
};

/**
 * 页表项结构
 */
struct page{
    u2 pageFrameNum;
    /**
     * 最低位：主存驻留标识
     * 次低位：引用标识
     * 次次低位：修改标识
     */
    u1 sign;
};

#endif //MEMORYMANAGEMENT_STRUCT_H
