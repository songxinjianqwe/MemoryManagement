//
// Created by songx on 2017/5/19.
//

#include "swap.h"
#include "page_bit_map.h"
#include "page_table.h"
#include <assert.h>
#include <stdio.h>

/**
 * 处理缺页中断：
 * 缺页中断处理程序会拿到页表项中的磁盘物理地址，查看内存空闲的页框数，
 * 如果有剩余，那么找到一个空闲的页框，将磁盘块上的数据拷贝到页框中，返回页框号，
 * 修改位示图和空闲块数，将引用位置1。以下步骤相同。	
 * 如果没有剩余，那么使用时钟算法（可以先使用从头遍历的方式做，之后再补充）淘汰一页。
 * 检查该页的修改标识，如果修改标识为1，就把该页框中的数据拷贝到对应的磁盘物理地址中，
 * 然后将主存驻留标识置0。之后再分配页框。
 * @param page 
 * @return 
 */
unsigned pageFaultInterrupt(unsigned diskAddress, m_pid_t pid) {
    //如果有剩余的页框，那么分配一个
    int pageFrameNum;
    if (isAllocatable()) {
        pageFrameNum = allocatePhysicalPage();
        assert(pageFrameNum >= 0);
        return pageFrameNum;
    } else {
        printf("进行页面调度\n");
        //如果没有剩余，那么进行调度，返回被换出的页号
        int swappedOutPageNum = clockPaging(pid);

        struct PageItem swappedOutPage = loadPage(swappedOutPageNum);
        //如果被换出的页框的修改位为1，那么需要将页框中的数据写回磁盘
        if (isPageModified(swappedOutPage.sign)) {
            swapOut(swappedOutPage.diskAddress,swappedOutPage.pageFrameNum);
        }
        pageFrameNum = swappedOutPage.pageFrameNum;
        swappedOutPage.pageFrameNum = 0;
        //将主存驻留标识置0
        clrbit(swappedOutPage.sign, PAGE_IN_MEMORY_INDEX);
        flushPage(swappedOutPage);
    }
    //将数据从磁盘拷贝到页框中
    swapIn(diskAddress,pageFrameNum);
    return pageFrameNum;
}

void initSwap() {
    struct SwapBitMap bitMap = loadSwapBitMap();
    bitMap.freeSwapPageSize = TOTAL_PAGE_NUM;
    for (unsigned i = 0; i < SWAP_BIT_MAP_SIZE; ++i) {
        bitMap.bits[i] = 0;
    }
    flushSwapBitMap(bitMap);
}


int allocateVirtualPage() {
    struct SwapBitMap bitMap = loadSwapBitMap();
    bitMap.freeSwapPageSize--;
    u1 bit;
    u1 mask = 128;//1000 0000
    //每次检查一个页
    for (unsigned i = 0; i < SWAP_BIT_MAP_SIZE; ++i) {
        bit = bitMap.bits[i];
        mask = 128;
        //检查连续的8个页
        for (unsigned j = 0; j < 8; ++j) {
            if (!(bit & mask)) {
                //将该页的占用位置1
                bitMap.bits[i] = bit | mask;
                flushSwapBitMap(bitMap);
                //返回页框号
                return i * 8 + j;
            }
            mask = mask >> 1;
        }
    }
    return OUT_OF_MEMORY;
}

void swapOut(unsigned diskAddress, unsigned pageFrameNum) {
    
}

void swapIn(unsigned diskAddress, unsigned pageFrameNum) {

}

void freeSwap() {

}

struct SwapBitMap loadSwapBitMap() {
    struct SwapBitMap bitMap;
    data_unit *ptr = (data_unit *) &bitMap;
    for (unsigned i = 0; i < SWAP_BIT_STRUCT_SIZE; ++i) {
        *(ptr + i) = mem_read(PAGE_BIT_STRUCT_SIZE + PAGE_TABLE_SIZE + i);
    }
    return bitMap;
}

void flushSwapBitMap(struct SwapBitMap bitMap) {
    data_unit *ptr = (data_unit *) &bitMap;
    for (unsigned i = 0; i < SWAP_BIT_STRUCT_SIZE; ++i) {
        mem_write(*(ptr + i), mem_read(PAGE_BIT_STRUCT_SIZE + PAGE_TABLE_SIZE + i));
    }
}