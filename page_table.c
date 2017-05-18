//
// Created by songx on 2017/5/17.
//

#include "type.h"
#include "struct.h"
#include "call.h"
#include "bottom.h"
#include <stdio.h>
#include "process.h"
#include "page_bit_map.h"
#include "page_table.h"
#include "address.h"
#include "stdbool.h"

struct PageTable loadPageTable() {
    struct PageTable table;
    data_unit *ptr;
    for (unsigned i = 0; i < TOTAL_PAGE_NUM; ++i) {
        //ptr指向每个页表项的开始
        ptr = (data_unit *) &table.pageItems[i];
        //读入一个页表项
        for (unsigned j = 0; j < PAGE_TABLE_ITEM_SIZE; ++j) {
            *(ptr + j) = mem_read(PAGE_BIT_STRUCT_SIZE + i * PAGE_TABLE_ITEM_SIZE + j);
        }
    }
    return table;
}

void flushPageTable(struct PageTable table) {
    data_unit *ptr;
    for (unsigned i = 0; i < TOTAL_PAGE_NUM; ++i) {
        //ptr指向每个页表项的开始
        ptr = (data_unit *) &table.pageItems[i];
        //读入每一个页表项
        for (unsigned j = 0; j < PAGE_TABLE_ITEM_SIZE; ++j) {
            mem_write(*(ptr + j), PAGE_BIT_STRUCT_SIZE + i * PAGE_TABLE_ITEM_SIZE + j);
        }
    }
}

void initPageTable() {
    struct PageTable pageTable = loadPageTable();
    for (unsigned i = 0; i < TOTAL_PAGE_NUM; ++i) {
        pageTable.pageItems[i].pageFrameNum = 0;
        pageTable.pageItems[i].sign = 0;
    }
    flushPageTable(pageTable);
}


int allocatePageFrames(unsigned pageSize) {
    struct PageTable pageTable = loadPageTable();
    bool isFree;
    unsigned pageStart;
    //找连续的pageSize个空闲页表项，如果没有，那么返回CONTINUED_PAGE_FRAME_NOT_FOUND
    for (pageStart = 0; pageStart < TOTAL_PAGE_NUM - pageSize; ++pageStart) {
        isFree = true;
        for (unsigned j = 0; j < pageSize; ++j) {
            if (pageTable.pageItems[pageStart + j].sign != 0) {
                isFree = false;
                break;
            }
        }
        if (isFree) {
            break;
        }
    }

    if (!isFree) {
        return CONTINUED_PAGE_FRAME_NOT_FOUND;
    }
    int pageFrameResult;
    //修改页表项
    for (unsigned i = 0; i < pageSize; ++i) {
        pageFrameResult = allocateOnePage();
        if (pageFrameResult < 0) {
            return pageFrameResult;
        } else {
            pageTable.pageItems[pageStart + i].pageFrameNum = pageFrameResult;
            //最后一位是占用位，置1
            pageTable.pageItems[pageStart + i].sign = pageTable.pageItems[pageStart + i].sign | 1;

        }
    }
    flushPageTable(pageTable);
    return pageStart;
}

struct PageItem loadPage(unsigned pageNum) {
    struct PageItem page;
    data_unit *ptr = (data_unit *) &page;
    for (unsigned i = 0; i < PAGE_TABLE_ITEM_SIZE; ++i) {
        *(ptr + i) = mem_read(PAGE_BIT_STRUCT_SIZE + pageNum * PAGE_TABLE_ITEM_SIZE + i);
    }
    return page;
}

void flushPage(struct PageItem page) {
    data_unit *ptr = (data_unit *) &page;
    for (unsigned i = 0; i < PAGE_TABLE_ITEM_SIZE; ++i) {
        mem_write(*(ptr + i), PAGE_BIT_STRUCT_SIZE + page.pageFrameNum * PAGE_TABLE_ITEM_SIZE + i);
    }
}


//由页号查页表
data_unit readPage(unsigned pageNum, unsigned offset) {
    struct PageItem page = loadPage(pageNum);
    //将页表项的引用位置1
    page.sign = page.sign | 0x4;
    flushPage(page);
    return mem_read(PAGE_FRAME_BEGIN_POS + combinePhyAddr(page.pageFrameNum, offset));
}

int writePage(data_unit data, unsigned pageNum, unsigned offset) {
    struct PageItem page = loadPage(pageNum);
    //将页表项的引用位置1
    page.sign = page.sign | 0x4;
    flushPage(page);
    mem_write(data, PAGE_FRAME_BEGIN_POS + combinePhyAddr(page.pageFrameNum, offset));
    return SUCCESS;
}

void freePageFrames(unsigned pageTableStart, unsigned pageSize) {
    struct PageTable pageTable = loadPageTable();
    for (unsigned i = 0; i < pageSize; ++i) {
        freeOnePage(pageTable.pageItems[pageTableStart + i].pageFrameNum);
        pageTable.pageItems[pageTableStart + i].pageFrameNum = 0;
        pageTable.pageItems[pageTableStart + i].sign = 0;
    }
    flushPageTable(pageTable);
}


/**
 * 如果访问的页号超过进程持有的页数，那么越界
 * 如果申请的内存不是页的大小的整数倍（多分配了一页的一部分），那么当访问最后一页的超过申请的内存的部分时，也是越界的
 * @param pcb 
 * @param address 
 * @return 是否越界
 */
bool isAccessFail(struct PCB pcb, v_address address) {
    int pageNum = parseToStartAddress(address);
    int offset = parseToOffset(address);
    if(pageNum >= pcb.pageSize){
        return true;
    }
    if(pcb.lastPageLimit != 0){
        if(pcb.pageSize - 1 == pageNum && offset >= pcb.lastPageLimit){
            return true;
        }
    }
    return false;
}