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
    bitMap.freePageFrameSize = TOTAL_PAGE_FRAME_NUM;
    for (unsigned i = 0; i < PAGE_BIT_MAP_SIZE; ++i) {
        bitMap.bits[i] = 0;
    }
    flushPageBitMap(bitMap);
}

bool isAllocatable() {
    u2 freePageFrameSize;
    data_unit *ptr = (data_unit *) &freePageFrameSize;
    for (unsigned i = 0; i < 2; ++i) {
        *(ptr + i) = mem_read(i);
    }
    return freePageFrameSize >= 1;
}

int allocatePhysicalPage() {
    struct PageBitMap bitMap = loadPageBitMap();
    bitMap.freePageFrameSize--;
    u1 bit;
    u1 mask = 128;//1000 0000
    //每次检查一个页框
    for (unsigned i = 0; i < PAGE_BIT_MAP_SIZE; ++i) {
        bit = bitMap.bits[i];
        mask = 128;
        //检查连续的8个页框
        for (unsigned j = 0; j < 8; ++j) {
            if (!(bit & mask)) {
                //将该页框的占用位置1
                bitMap.bits[i] = bit | mask;
                flushPageBitMap(bitMap);
                //返回页框号
                return i * 8 + j;
            }
            mask = mask >> 1;
        }
    }
    return OUT_OF_MEMORY;
}

void freePhysicalPage(unsigned pageFrameNum) {
    struct PageBitMap bitMap = loadPageBitMap();
    bitMap.freePageFrameSize++;
    //pageFrameNum是页框号，页框号除8得到所在的字节，然后余数就是第i位
    //余数为1->第6位置0（从右往左）
    //7-1 = 6
    //1<<6 = 0100 0000
    //~(1<<6) = 1011 1111 
    //与~(1<<6)做与运算即可将第6位置0
    clrbit(bitMap.bits[pageFrameNum / 8], (7 - (pageFrameNum % 8)));
    flushPageBitMap(bitMap);
}