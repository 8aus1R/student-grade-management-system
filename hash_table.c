#include "hash_table.h"

/* BKDR�ַ�����ϣ���� */
static unsigned int hash_key(const char* key) {
    unsigned int seed = 131;
    unsigned int hash = 0;
    while (*key) {
        hash = hash * seed + (*key++);
    }
    return hash % HASH_SIZE;
}

HashTable* hash_create() {
    int i;
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) return NULL;
    for (i = 0; i < HASH_SIZE; i++) {
        table->buckets[i] = NULL;
    }
    table->size = 0;
    return table;
}

void hash_destroy(HashTable* table) {
    int i;
    if (!table) return;
    for (i = 0; i < HASH_SIZE; i++) {
        HashNode* cur = table->buckets[i];
        while (cur) {
            HashNode* tmp = cur;
            cur = cur->next;
            free(tmp);
        }
    }
    free(table);
}

int hash_insert(HashTable* table, const Record* rec) {
    char key[ID_LEN + COURSE_ID_LEN];
    unsigned int idx;
    HashNode* node;

    if (!table || !rec) return ERROR;

    get_record_key(rec, key);
    idx = hash_key(key);

    node = (HashNode*)malloc(sizeof(HashNode));
    if (!node) return ERROR;
    node->data = *rec;
    node->next = table->buckets[idx];
    table->buckets[idx] = node;
    table->size++;
    return OK;
}

int hash_delete(HashTable* table, const char* student_id, const char* course_id) {
    char key[ID_LEN + COURSE_ID_LEN];
    Record tmp;
    unsigned int idx;
    HashNode *cur, *prev;

    if (!table || !student_id || !course_id) return ERROR;

    strcpy(tmp.student_id, student_id);
    strcpy(tmp.course_id, course_id);
    get_record_key(&tmp, key);
    idx = hash_key(key);

    cur = table->buckets[idx];
    prev = NULL;
    while (cur) {
        if (strcmp(cur->data.student_id, student_id) == 0 &&
            strcmp(cur->data.course_id, course_id) == 0) {
            if (prev) prev->next = cur->next;
            else table->buckets[idx] = cur->next;
            free(cur);
            table->size--;
            return OK;
        }
        prev = cur;
        cur = cur->next;
    }
    return NOT_FOUND;
}

Record* hash_find(HashTable* table, const char* student_id, const char* course_id) {
    char key[ID_LEN + COURSE_ID_LEN];
    Record tmp;
    unsigned int idx;
    HashNode* cur;

    if (!table || !student_id || !course_id) return NULL;

    strcpy(tmp.student_id, student_id);
    strcpy(tmp.course_id, course_id);
    get_record_key(&tmp, key);

    idx = hash_key(key);
    cur = table->buckets[idx];
    while (cur) {
        if (strcmp(cur->data.student_id, student_id) == 0 &&
            strcmp(cur->data.course_id, course_id) == 0) {
            return &cur->data;
        }
        cur = cur->next;
    }
    return NULL;
}

/* �޸ļ�¼�ɼ� */
int hash_update(HashTable* table, const char* student_id, const char* course_id, int new_score) {
    Record* rec;
    if (!table || !student_id || !course_id) return ERROR;
    rec = hash_find(table, student_id, course_id);
    if (!rec) return NOT_FOUND;
    rec->score = new_score;
    return OK;
}

void hash_traverse(HashTable* table, void (*visit)(const Record*)) {
    int i;
    if (!table || !visit) return;
    for (i = 0; i < HASH_SIZE; i++) {
        HashNode* cur = table->buckets[i];
        while (cur) {
            visit(&cur->data);
            cur = cur->next;
        }
    }
}

int hash_size(HashTable* table) {
    return table ? table->size : 0;
}
