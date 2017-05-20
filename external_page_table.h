//
// Created by songx on 2017/5/19.
//

#ifndef MEMORYMANAGEMENT_EXTERNAL_PAGE_TABLE_H
#define MEMORYMANAGEMENT_EXTERNAL_PAGE_TABLE_H
#include "struct.h"

void initExternalPageTable();
struct ExternalPageTable loadExternalPageTable();
void flushExternalPageTable(struct ExternalPageTable table);
struct ExternalPageItem loadExternalPage(unsigned pageNum);

#endif //MEMORYMANAGEMENT_EXTERNAL_PAGE_TABLE_H
