#ifndef DATA_GENERATOR_H
#define DATA_GENERATOR_H

#include "common.h"

/* 生成指定数量的合法选课记录 */
Record* generate_records(int count, int* actual_count);

/* 释放生成的记录数组 */
void free_records(Record* records);

#endif
