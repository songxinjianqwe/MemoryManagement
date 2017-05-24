//
// Created by songx on 2017/5/16.
//

#ifndef MEMORYMANAGEMENT_STRUCT_H
#define MEMORYMANAGEMENT_STRUCT_H

#include "bottom.h"
//***************************位运算宏***************************************************************************//

#define setbit(x, y) x|=(1<<y) //将X的第Y位置1
#define clrbit(x, y) x&=~(1<<y) //将X的第Y位清0

//*********************内存模型***********************************************************************************//
//1、内存的位示图
//2、页表
//3、外页表
//4、进程表
//5、交换区的位示图
//6、页框们
//*****************************常量定义****************************************************************************//

#define PAGE_FRAME_SIZE (1024*4)  //页框大小为4KB 4096

#define TOTAL_PAGE_FRAME_NUM (MEMORY_SIZE/PAGE_FRAME_SIZE) //共有2^15个页框，适应物理内存的大小 32768

#define PAGE_BIT_MAP_SIZE (TOTAL_PAGE_FRAME_NUM/8)    //页框的位示图占2^12个字节 4096

#define PAGE_BIT_STRUCT_SIZE  (sizeof(struct PageBitMap )) //位示图结构体的大小为 4096+2个字节

#define PAGE_TABLE_ITEM_SIZE (sizeof(struct PageItem)) //页表项的长度为8字节  8

#define TOTAL_PAGE_NUM  (DISK_SIZE/PAGE_FRAME_SIZE) //可供分配的页数，页表项数适应虚拟内存的大小 131072

#define PAGE_TABLE_SIZE (TOTAL_PAGE_NUM*PAGE_TABLE_ITEM_SIZE) //页表的大小为2^17字节 1048576 1MB

#define EXTERNAL_PAGE_TABLE_ITEM_SIZE (sizeof(struct ExternalPageItem)) //外页表项的大小 

#define EXTERNAL_PAGE_TABLE_SIZE (TOTAL_PAGE_NUM*EXTERNAL_PAGE_TABLE_ITEM_SIZE) //外页表的大小

#define PROCESS_NUM 1024 //进程总数为1024个 1024

#define PCB_SIZE (sizeof(struct PCB)) //进程控制块的大小为16个字节 16 

#define PCB_TABLE_SIZE (PROCESS_NUM*PCB_SIZE) //进程表的大小为16384字节 

//假设整个磁盘都是交换区，对交换区进行管理
#define SWAP_BIT_MAP_SIZE (TOTAL_PAGE_NUM/8)  //交换区的位示图共16384个字节

#define SWAP_BIT_STRUCT_SIZE (sizeof(struct SwapBitMap)) //交换区位示图结构体的大小为16388个字节

#define PAGE_FRAME_BEGIN_POS  (PAGE_BIT_STRUCT_SIZE+PAGE_TABLE_SIZE+EXTERNAL_PAGE_TABLE_SIZE+SWAP_BIT_STRUCT_SIZE+PCB_TABLE_SIZE) //页框的开始地址 1085446

#define AVAILABLE_PAGE_FRAME_NUM ((MEMORY_SIZE-PAGE_FRAME_BEGIN_POS)/PAGE_FRAME_SIZE) //32502

#define AVERAGE_PAGE_NUM_PER_PROCESS (AVAILABLE_PAGE_FRAME_NUM/PROCESS_NUM) //31

//*************************错误状态码*********************************************************************//
#define  OUT_OF_MEMORY -10 //内存不足

#define  PID_DUPLICATED -20  //PID已被占用

#define  CONTINUED_PAGE_FRAME_NOT_FOUND  -30  //没有足够的连续的页表项

#define  SUCCESS 0 //成功

#define  PID_NOT_FOUND -40  //未找到此进程

#define  ACCESS_FAIL -1 //地址越界

//***********************位位置常量***********************************************************//
#define  PAGE_USED_INDEX 0
#define  PAGE_IN_MEMORY_INDEX 1
#define  PAGE_REFERRED_INDEX 2
#define  PAGE_MODIFIED_INDEX 3

//**************************类型重命名*****************************************************************//


typedef unsigned int u4;
typedef unsigned short u2;
typedef unsigned char u1;


//*************************************结构定义*******************************************************//
/**
 * 进程结构
 */
struct PCB {
    u2 pid;
    u2 lastPageLimit;
    u4 pageSize;
    u4 pageTableStart;
    u4 pageTablePtr;
};

/**
 * 进程表
 */
struct PCBTable {
    struct PCB pcbs[PROCESS_NUM];
};

/**
 * 位示图管理页框的占用情况
 */
struct PageBitMap {
    u2 freePageFrameSize;
    u1 bits[PAGE_BIT_MAP_SIZE];
};

/**
 * 页表项结构，4字节一个表项
 * 如果pageFrameNum为-1，那么表示该页表项未被占用
 */
struct PageItem {
    u2 pageFrameNum;

    /**最低位：该页表项是否被使用
     * 次低位：主存驻留标识
     * 次次低位：引用标识
     * 次次次低位：修改标识
     */
    u2 sign;
};

/**
 * 页表
 */
struct PageTable {
    struct PageItem pageItems[TOTAL_PAGE_NUM];
};

/**
 * 管理交换区的位示图
 */
struct SwapBitMap {
    u4 freeSwapPageSize;
    u1 bits[SWAP_BIT_MAP_SIZE];
};
/**
 * 外页表项
 */
struct ExternalPageItem{
    u4 diskPageNum;
};

/**
 * 外页表
 */
struct ExternalPageTable{
    struct ExternalPageItem pageItems[TOTAL_PAGE_NUM];
};


#endif //MEMORYMANAGEMENT_STRUCT_H
