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


void test() {
    init();
    v_address va;
    int result = allocate(&va, 65536, 1);
    printf("RESULT: %d\n", result);
    printf("VIRTUAL_ADDR: %u\n", va);
    struct PageBitMap bitMap = loadPageBitMap();
    printf("freePageFrameSize:%d\n", bitMap.freePageFrameSize);
    printf("bitMap.bits[0]: %d\n",bitMap.bits[0]);
    printf("bitMap.bits[1]: %d\n",bitMap.bits[1]);
    printf("bitMap.bits[2]: %d\n",bitMap.bits[2]);
    
    struct PageTable table = loadPageTable();
    for(unsigned i = 0; i < 16;++i){
        printf("table.pageItems[%d].pageFrameNum: %d\n",i,table.pageItems[i].pageFrameNum);
        printf("table.pageItems[%d].sign: %d\n",i,table.pageItems[i].sign);
    }
}

int main() {
//    u1 bit = 0;
//    u1 mask = 128;
//    printf("%d\n",bit&mask);
    test();
    return 0;
}