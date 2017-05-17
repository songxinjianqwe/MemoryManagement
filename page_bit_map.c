//
// Created by songx on 2017/5/17.
//

#include <stdio.h>
#include "type.h"
#include "struct.h"
#include "call.h"
#include "bottom.h"
#include "process.h"
#include "page_bit_map.h"
#include "page_table.h"

struct PageBitMap loadPageBitMap() {
    struct PageBitMap bitMap;
    data_unit *ptr = (data_unit *) &bitMap;
    for (unsigned i = 0; i < PAGE_BIT_STRUCT_SIZE; ++i) {
        *(ptr + i) = mem_read(i);
    }
    return bitMap;
}

void flushPageBitMap(struct PageBitMap bitMap) {
    data_unit *ptr = (data_unit *) &bitMap;
    for (unsigned i = 0; i < PAGE_BIT_STRUCT_SIZE; ++i) {
        mem_write(*(ptr + i), i);
    }
}

void initPageBitMap() {
    struct PageBitMap bitMap = loadPageBitMap();
    bitMap.freePageFrameSize = TOTAL_PAGE_NUM;
    for (unsigned i = 0; i < PAGE_BIT_MAP_SIZE; ++i) {
        bitMap.bits[i] = 0;
    }
    flushPageBitMap(bitMap);
}

bool isAllocatable(m_size_t size) {
    struct PageBitMap bitMap = loadPageBitMap();
    return bitMap.freePageFrameSize >= (size / PAGE_FRAME_SIZE);
}

