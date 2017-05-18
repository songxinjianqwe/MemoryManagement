//
// Created by songx on 2017/5/18.
//
#include "address.h"

unsigned parseToStartAddress(v_address address){
    return address >> 12;
}

unsigned parseToOffset(v_address address){
    return address & 0xFFF;
}

p_address combinePhyAddr(unsigned pageNum, unsigned offset){
    return pageNum & 0xFFFFF0000 + offset;    
}