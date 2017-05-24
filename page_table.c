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
#include "swap.h"
#include "external_page_table.h"

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
            if (isPageUsed(pageTable.pageItems[pageStart + j].sign)) {
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
//    return allocateWithSwap(pageSize, pageTable, pageStart);

    //如果物理页框足够，那么直接分配物理页框
    if (isAllocatable(pageSize)/* && pageSize <= AVERAGE_PAGE_NUM_PER_PROCESS*/) {
        return allocateAllInMemory(pageSize, pageTable, pageStart);
    } else {
        //否则采用请求式分页存储
        return allocateWithSwap(pageSize, pageTable, pageStart);
    }
}

/**
 * 直接分配页框
 * @param pageSize 
 * @param pageTable 
 * @param pageStart 
 * @return 
 */
int allocateAllInMemory(unsigned pageSize, struct PageTable pageTable, unsigned pageStart) {
    int pageFrameResult = 0;
    //修改页表项
    for (unsigned i = 0; i < pageSize; ++i) {
        pageFrameResult = allocatePhysicalPage();
        if (pageFrameResult < 0) {
            return pageFrameResult;
        } else {
            pageTable.pageItems[pageStart + i].pageFrameNum = pageFrameResult;
            //置主存驻留标识为1
            setbit(pageTable.pageItems[pageStart + i].sign, PAGE_IN_MEMORY_INDEX);
            //最后一位是占用位，置1
            setbit(pageTable.pageItems[pageStart + i].sign, PAGE_USED_INDEX);
        }
    }
    flushPageTable(pageTable);
    return pageFrameResult;
}

/**
 * 只为第一页分配页框
 * @param pageSize 
 * @param pageTable 
 * @param pageStart 
 * @return 
 */
int allocateWithSwap(unsigned pageSize, struct PageTable pageTable, unsigned pageStart) {
    int pageFrameResult;
    //读入外页表
    struct ExternalPageTable externalPageTable = loadExternalPageTable();
    //修改页表项
    for (unsigned i = 0; i < pageSize; ++i) {
        //只为第一页分配页框
        if (i == 0) {
            pageFrameResult = allocatePhysicalPage();
            if (pageFrameResult < 0) {
                return pageFrameResult;
            } else {
                pageTable.pageItems[pageStart + i].pageFrameNum = pageFrameResult;
                //置主存驻留标识为1
                setbit(pageTable.pageItems[pageStart + i].sign, PAGE_IN_MEMORY_INDEX);
                //第一次装入时引用位置1
                setbit(pageTable.pageItems[pageStart + i].sign, PAGE_REFERRED_INDEX);
            }
        } else {
            //其他页不分配页框
            pageTable.pageItems[pageStart + i].pageFrameNum = 0;
            //置主存驻留标识为0
            clrbit(pageTable.pageItems[pageStart + i].sign, PAGE_IN_MEMORY_INDEX);
        }
        //从磁盘中找一页空闲的区域，每页都会有对应的虚存页
        int diskPageResult = allocateVirtualPage();
        if (diskPageResult < 0) {
            return diskPageResult;
        }
        externalPageTable.pageItems[pageStart + i].diskPageNum = diskPageResult;

        //占用位置1
        setbit(pageTable.pageItems[pageStart + i].sign, PAGE_USED_INDEX);
    }
    flushPageTable(pageTable);
    flushExternalPageTable(externalPageTable);
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

void flushPage(unsigned pageNum, struct PageItem page) {
    data_unit *ptr = (data_unit *) &page;
    for (unsigned i = 0; i < PAGE_TABLE_ITEM_SIZE; ++i) {
        mem_write(*(ptr + i), PAGE_BIT_STRUCT_SIZE + pageNum * PAGE_TABLE_ITEM_SIZE + i);
    }
}


//由页号查页表
data_unit readPage(unsigned pageNum, unsigned offset, m_pid_t pid) {
    struct PageItem page = loadPage(pageNum);
    struct ExternalPageItem externalPageItem = loadExternalPage(pageNum);
    //如果主存驻留标识为0
    if (!isPageInMainMemory(page.sign)) {
        //发出缺页中断
        page.pageFrameNum = pageFaultInterrupt(externalPageItem.diskPageNum, pid);
        //将页表项的主存驻留标识置1
        setbit(page.sign, PAGE_IN_MEMORY_INDEX);
    }
    //将页表项的引用位置1
    setbit(page.sign, PAGE_REFERRED_INDEX);
    flushPage(pageNum, page);
    return mem_read(PAGE_FRAME_BEGIN_POS + combinePhyAddr(page.pageFrameNum, offset));
}

int writePage(data_unit data, unsigned pageNum, unsigned offset, m_pid_t pid) {
    struct PageItem page = loadPage(pageNum);
    struct ExternalPageItem externalPageItem = loadExternalPage(pageNum);
    if (!isPageInMainMemory(page.sign)) {
        //发出缺页中断
        page.pageFrameNum = pageFaultInterrupt(externalPageItem.diskPageNum, pid);
        //将页表项的主存驻留标识置1
        setbit(page.sign, PAGE_IN_MEMORY_INDEX);
    }
    //将页表项的引用位置1
    setbit(page.sign, PAGE_REFERRED_INDEX);
    //将页表项的修改位置1
    setbit(page.sign, PAGE_MODIFIED_INDEX);
    flushPage(pageNum, page);
    mem_write(data, PAGE_FRAME_BEGIN_POS + combinePhyAddr(page.pageFrameNum, offset));
    return SUCCESS;
}

void freePageFrames(unsigned pageTableStart, unsigned pageSize) {
    struct PageTable pageTable = loadPageTable();
    struct ExternalPageTable externalPageTable = loadExternalPageTable();
    for (unsigned i = 0; i < pageSize; ++i) {
        //如果主存驻留标识为1，那么就释放该页框
        if (isPageInMainMemory(pageTable.pageItems[pageTableStart + i].sign)) {
            freePhysicalPage(pageTable.pageItems[pageTableStart + i].pageFrameNum);
        }
        //释放对应的磁盘空间
        freeVirtualPage(externalPageTable.pageItems[pageTableStart + i].diskPageNum);
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
    if (pageNum >= pcb.pageSize) {
        return true;
    }
    if (pcb.lastPageLimit != 0) {
        if (pcb.pageSize - 1 == pageNum && offset >= pcb.lastPageLimit) {
            return true;
        }
    }
    return false;
}

//unsigned clockPaging(m_pid_t pid) {
//    struct PCB pcb = loadPCB(pid);
//    unsigned pageTableStart = pcb.pageTableStart;
//    struct PageTable pageTable = loadPageTable();
//    for (unsigned i = 0; i < pcb.pageSize; ++i) {
//        //如果引用位为0
//        if (!isPageReferred(pageTable.pageItems[pageTableStart + i].sign)) {
//            return pageTableStart + i;
//        }
//    }
//    return pageTableStart;
//}
//
//改进后的时钟分页调度算法
unsigned clockPaging(m_pid_t pid) {
    struct PCB pcb = loadPCB(pid);
    unsigned ptr = pcb.pageTablePtr;
    unsigned end = pcb.pageTableStart + pcb.pageTablePtr;
    struct PageTable pageTable = loadPageTable();
    unsigned sign;
    //如果从指针开始位置，存在既没有被引用，也没有被修改的页，那么它将被换出
    while (true) {
        for (; ptr < end; ++ptr) {
            sign = pageTable.pageItems[ptr].sign;
            if (isPageInMainMemory(sign) && !isPageReferred(sign)
                && !isPageModified(sign)) {
                //找到被淘汰的页时，指针需要推进一步
                pcb.pageTablePtr = ptr + 1;
                flushPCB(pcb);
                return ptr;
            }
        }

        //如果不存在，那么指针仍指向原来位置
        ptr = pcb.pageTablePtr;
        
        //查找存在没有被引用，但被修改过的页
        for (; ptr < end; ++ptr) {
            sign = pageTable.pageItems[ptr].sign;
            if (isPageInMainMemory(sign) && !isPageReferred(sign)
                && isPageModified(sign)) {
                pcb.pageTablePtr = ptr + 1;
                flushPCB(pcb);
                return ptr;
            } else {
                //将途经的页面的引用位置0
                clrbit(pageTable.pageItems[ptr].sign, PAGE_REFERRED_INDEX);
            }
        }
        //如果仍不存在，那么指针仍指向原来位置
        ptr = pcb.pageTablePtr;
    }

}


bool isPageUsed(u2 sign) {
    return sign & 1;
}

bool isPageInMainMemory(u2 sign) {
    return sign & 2;
}

bool isPageReferred(u2 sign) {
    return sign & 4;
}

bool isPageModified(u2 sign) {
    return sign & 8;
}
