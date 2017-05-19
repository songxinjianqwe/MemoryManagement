//
// Created by songx on 2017/5/19.
//
#include "external_page_table.h"

void initExternalPageTable() {
    struct ExternalPageTable pageTable = loadExternalPageTable();
    for (unsigned i = 0; i < TOTAL_PAGE_NUM; ++i) {
        pageTable.pageItems[i].diskPageNum = 0;
    }
    flushExternalPageTable(pageTable);
}

struct ExternalPageTable loadExternalPageTable() {
    struct ExternalPageTable table;
    data_unit *ptr;
    for (unsigned i = 0; i < TOTAL_PAGE_NUM; ++i) {
        //ptr指向每个页表项的开始
        ptr = (data_unit *) &table.pageItems[i];
        //读入一个页表项
        for (unsigned j = 0; j < PAGE_TABLE_ITEM_SIZE; ++j) {
            *(ptr + j) = mem_read(PAGE_BIT_STRUCT_SIZE + PAGE_TABLE_SIZE + i * EXTERNAL_PAGE_TABLE_ITEM_SIZE + j);
        }
    }
    return table;
}

void flushExternalPageTable(struct ExternalPageTable table) {
    data_unit *ptr;
    for (unsigned i = 0; i < TOTAL_PAGE_NUM; ++i) {
        //ptr指向每个页表项的开始
        ptr = (data_unit *) &table.pageItems[i];
        //读入每一个页表项
        for (unsigned j = 0; j < PAGE_TABLE_ITEM_SIZE; ++j) {
            mem_write(*(ptr + j), PAGE_BIT_STRUCT_SIZE + PAGE_TABLE_SIZE + i * EXTERNAL_PAGE_TABLE_ITEM_SIZE + j);
        }
    }
}

struct ExternalPageItem loadExternalPage(unsigned pageNum) {
    struct ExternalPageItem page;
    data_unit *ptr = (data_unit *) &page;
    for (unsigned i = 0; i < EXTERNAL_PAGE_TABLE_ITEM_SIZE; ++i) {
        *(ptr + i) = mem_read(PAGE_BIT_STRUCT_SIZE + PAGE_TABLE_SIZE + pageNum * EXTERNAL_PAGE_TABLE_ITEM_SIZE + i);
    }
    return page;
}

void flushExternalPage(unsigned pageNum,struct ExternalPageItem page) {
    data_unit *ptr = (data_unit *) &page;
    for (unsigned i = 0; i < EXTERNAL_PAGE_TABLE_ITEM_SIZE; ++i) {
        mem_write(*(ptr + i), PAGE_BIT_STRUCT_SIZE + PAGE_TABLE_SIZE + pageNum * EXTERNAL_PAGE_TABLE_ITEM_SIZE + i);
    }
}