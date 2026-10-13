#ifndef DATA_GENERATOR_H
#define DATA_GENERATOR_H

#include "common.h"

/* ����ָ�������ĺϷ�ѡ�μ�¼ */
Record* generate_records(int count, int* actual_count);

/* �ͷ����ɵļ�¼���� */
void free_records(Record* records);

#endif
