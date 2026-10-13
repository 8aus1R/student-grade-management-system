#include "linked_list.h"

LinkedList* list_create() {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    if (!list) return NULL;
    list->head = list->tail = NULL;
    list->size = 0;
    return list;
}

void list_destroy(LinkedList* list) {
    ListNode* cur;
    if (!list) return;
    cur = list->head;
    while (cur) {
        ListNode* tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    free(list);
}

int list_insert(LinkedList* list, const Record* rec) {
    ListNode* node;
    if (!list || !rec) return ERROR;

    node = (ListNode*)malloc(sizeof(ListNode));
    if (!node) return ERROR;
    node->data = *rec;
    node->prev = node->next = NULL;

    if (list->size == 0) {
        list->head = list->tail = node;
    } else {
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
    return OK;
}

int list_delete(LinkedList* list, const char* student_id, const char* course_id) {
    ListNode* cur;
    if (!list || !student_id || !course_id) return ERROR;

    cur = list->head;
    while (cur) {
        if (strcmp(cur->data.student_id, student_id) == 0 &&
            strcmp(cur->data.course_id, course_id) == 0) {
            if (cur->prev) cur->prev->next = cur->next;
            else list->head = cur->next;
            if (cur->next) cur->next->prev = cur->prev;
            else list->tail = cur->prev;

            free(cur);
            list->size--;
            return OK;
        }
        cur = cur->next;
    }
    return NOT_FOUND;
}

int list_update(LinkedList* list, const char* student_id, const char* course_id, int new_score) {
    ListNode* cur;
    if (!list || !student_id || !course_id) return ERROR;

    cur = list->head;
    while (cur) {
        if (strcmp(cur->data.student_id, student_id) == 0 &&
            strcmp(cur->data.course_id, course_id) == 0) {
            cur->data.score = new_score;
            return OK;
        }
        cur = cur->next;
    }
    return NOT_FOUND;
}

Record* list_find(LinkedList* list, const char* student_id, const char* course_id) {
    ListNode* cur;
    if (!list || !student_id || !course_id) return NULL;

    cur = list->head;
    while (cur) {
        if (strcmp(cur->data.student_id, student_id) == 0 &&
            strcmp(cur->data.course_id, course_id) == 0) {
            return &cur->data;
        }
        cur = cur->next;
    }
    return NULL;
}

void list_traverse(LinkedList* list, void (*visit)(const Record*)) {
    ListNode* cur;
    if (!list || !visit) return;
    cur = list->head;
    while (cur) {
        visit(&cur->data);
        cur = cur->next;
    }
}

int list_size(LinkedList* list) {
    return list ? list->size : 0;
}

int list_sort_by_score(LinkedList* list, bool descending) {
    int i, j;
    ListNode* p;
    if (!list || list->size < 2) return OK;

    for (i = 0; i < list->size - 1; i++) {
        p = list->head;
        for (j = 0; j < list->size - 1 - i; j++) {
            int swap_flag = 0;
            if (descending) {
                if (p->data.score < p->next->data.score) swap_flag = 1;
            } else {
                if (p->data.score > p->next->data.score) swap_flag = 1;
            }

            if (swap_flag) {
                Record tmp = p->data;
                p->data = p->next->data;
                p->next->data = tmp;
            }
            p = p->next;
        }
    }
    return OK;
}
