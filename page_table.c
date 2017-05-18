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
            mem_write(*(ptr + j),PAGE_BIT_STRUCT_SIZE + i * PAGE_TABLE_ITEM_SIZE + j);
        }
    }
}

void initPageTable() {
    struct PageTable pageTable = loadPageTable();
    for(unsigned i = 0 ; i < TOTAL_PAGE_NUM;++i){
        pageTable.pageItems[i].pageFrameNum = 0;
        pageTable.pageItems[i].sign = 0;
    }
    flushPageTable(pageTable);
}


int allocatePageFrames(unsigned pageSize){
    struct PageTable pageTable = loadPageTable();
    bool isFree;
    unsigned pageStart;
    //找连续的pageSize个空闲页表项，如果没有，那么返回CONTINUED_PAGE_FRAME_NOT_FOUND
    for(pageStart = 0; pageStart < TOTAL_PAGE_NUM - pageSize;++pageStart){
        isFree = true;
        for(unsigned j = 0; j < pageSize;++j){
            if(pageTable.pageItems[pageStart+j].sign != 0){
                isFree = false;
                break;
            }
        }
        if(isFree){
            break;
        }
    }
    
    if(!isFree){
        return CONTINUED_PAGE_FRAME_NOT_FOUND;
    }
    int pageFrameResult;
    //修改位示图
    for(unsigned i = 0; i < pageSize;++i){
        pageFrameResult = allocateOnePage();
        if(pageFrameResult < 0){
            return pageFrameResult;
        }else{
            pageTable.pageItems[pageStart+i].pageFrameNum = pageFrameResult;
            //最后一位是占用位，置1
            pageTable.pageItems[pageStart+i].sign = pageTable.pageItems[pageStart+i].sign | 1;
            
        }
    }
    flushPageTable(pageTable);
    return pageStart;
}