//
// Created by songx on 2017/5/18.
//

#ifndef MEMORYMANAGEMENT_ADDRESS_H
#define MEMORYMANAGEMENT_ADDRESS_H
#include "type.h"
/**
 * 取虚拟地址的前20位，得到页号
 * @param address 
 * @return 
 */
unsigned parseToStartAddress(v_address address);
/**
 * 取虚拟地址的后12位，得到页内偏移
 * @param address 
 * @return 
 */
unsigned parseToOffset(v_address address);

p_address combinePhyAddr(unsigned pageNum, unsigned offset);

#endif //MEMORYMANAGEMENT_ADDRESS_H
