#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "common.h"

typedef struct ListNode {
    Record data;
    struct ListNode* prev;
    struct ListNode* next;
} ListNode;

typedef struct {
    ListNode* head;
    ListNode* tail;
    int size;
} LinkedList;

LinkedList* list_create();
void list_destroy(LinkedList* list);

int list_insert(LinkedList* list, const Record* rec);
int list_delete(LinkedList* list, const char* student_id, const char* course_id);
int list_update(LinkedList* list, const char* student_id, const char* course_id, int new_score);
Record* list_find(LinkedList* list, const char* student_id, const char* course_id);
void list_traverse(LinkedList* list, void (*visit)(const Record*));
int list_sort_by_score(LinkedList* list, bool descending);
int list_size(LinkedList* list);

#endif
