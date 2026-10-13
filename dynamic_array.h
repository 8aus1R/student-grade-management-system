#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "common.h"

typedef struct {
    Record* data;       /* �洢���� */
    int size;           /* ��ǰ��¼�� */
    int capacity;       /* ��ǰ���� */
} DynamicArray;

DynamicArray* darray_create();
void          darray_destroy(DynamicArray* arr);

int      darray_insert(DynamicArray* arr, const Record* rec);
int      darray_delete(DynamicArray* arr, const char* sid, const char* cid);
int      darray_update(DynamicArray* arr, const char* sid, const char* cid, int new_score);
Record*  darray_find(DynamicArray* arr, const char* sid, const char* cid);
void     darray_traverse(DynamicArray* arr, void (*visit)(const Record*));
int      darray_size(DynamicArray* arr);

#endif
