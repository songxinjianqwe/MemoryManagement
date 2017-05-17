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
        pageTable.pageItems[i].sign[0] = 0;
        pageTable.pageItems[i].sign[1] = 0;
    }
    flushPageTable(pageTable);
}