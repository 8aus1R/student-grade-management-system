#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "common.h"

#define HASH_SIZE 10007  /* ��ϣͰ������������ */

typedef struct HashNode {
    Record data;
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode* buckets[HASH_SIZE];
    int size;
} HashTable;

HashTable* hash_create();
void hash_destroy(HashTable* table);

int hash_insert(HashTable* table, const Record* rec);
int hash_delete(HashTable* table, const char* student_id, const char* course_id);
Record* hash_find(HashTable* table, const char* student_id, const char* course_id);
int hash_update(HashTable* table, const char* student_id, const char* course_id, int new_score);
void hash_traverse(HashTable* table, void (*visit)(const Record*));
int hash_size(HashTable* table);

#endif
