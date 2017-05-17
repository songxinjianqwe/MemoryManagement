//
// Created by songx on 2017/5/16.
//

#ifndef MEMORYMANAGEMENT_STRUCT_H
#define MEMORYMANAGEMENT_STRUCT_H
#include "call.h"
#include "bottom.h"

#define PAGE_FRAME_SIZE (1024*4)  //页框大小为4KB 4096

#define TOTAL_PAGE_NUM (MEMORY_SIZE/PAGE_FRAME_SIZE) //共有2^15个页框 32768

#define PAGE_BIT_MAP_SIZE (TOTAL_PAGE_NUM/8)    //页框的位示图占2^12个字节 4096

#define PAGE_TABLE_ITEM_SIZE (sizeof(struct page)) //页表项的长度为4字节  4

#define PAGE_TABLE_SIZE (TOTAL_PAGE_NUM*PAGE_TABLE_ITEM_SIZE) //页表的大小为2^17字节 131072

#define PROCESS_NUM 1024 //进程总数为1024个 1024

#define PCB_SIZE (sizeof(struct PCB)) //进程控制块的大小为8个字节 8

#define PCB_TABLE_SIZE (PROCESS_NUM*PCB_SIZE) //进程表的大小为2^13 字节 8192

#define PAGE_FRAME_BEGIN_POS  (2+PAGE_BIT_MAP_SIZE+PAGE_TABLE_SIZE+PCB_TABLE_SIZE) //页框的开始地址 143362


typedef unsigned int u4;
typedef unsigned short u2;
typedef unsigned char u1;

/**
 * 进程结构
 */
struct PCB{
    u2 pid;
    u2 pageSize;
    u4 pageTableStart;
};

/**
 * 位示图管理页框的占用情况
 */
struct pageBitMap{
    u2 freePageFrame;
    u1 bits[PAGE_BIT_MAP_SIZE];
};

/**
 * 页表项结构，4字节一个表项
 */
struct page{
    u2 pageFrameNum;
    /**
     * 最低位：主存驻留标识
     * 次低位：引用标识
     * 次次低位：修改标识
     */
    u1 sign[2];
};



#endif //MEMORYMANAGEMENT_STRUCT_H
