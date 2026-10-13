#include "dynamic_array.h"

#define DARRAY_INIT_CAP 64

DynamicArray* darray_create() {
    DynamicArray* arr = (DynamicArray*)malloc(sizeof(DynamicArray));
    if (!arr) return NULL;
    arr->data = (Record*)malloc(DARRAY_INIT_CAP * sizeof(Record));
    if (!arr->data) { free(arr); return NULL; }
    arr->size = 0;
    arr->capacity = DARRAY_INIT_CAP;
    return arr;
}

void darray_destroy(DynamicArray* arr) {
    if (!arr) return;
    if (arr->data) free(arr->data);
    free(arr);
}

static int darray_grow(DynamicArray* arr) {
    int new_cap;
    Record* new_data;
    if (!arr) return ERROR;
    new_cap = arr->capacity * 2;
    new_data = (Record*)realloc(arr->data, new_cap * sizeof(Record));
    if (!new_data) return ERROR;
    arr->data = new_data;
    arr->capacity = new_cap;
    return OK;
}

/* 二分查找：返回匹配记录的索引，未找到返回 -1 */
static int binary_search(DynamicArray* arr, const char* sid, const char* cid) {
    int low = 0;
    int high = arr->size - 1;
    Record key;
    int cmp;

    strcpy(key.student_id, sid);
    strcpy(key.course_id, cid);

    while (low <= high) {
        int mid = low + (high - low) / 2;
        cmp = record_key_compare(&key, &arr->data[mid]);
        if (cmp == 0) return mid;
        else if (cmp < 0) high = mid - 1;
        else low = mid + 1;
    }
    return -1;
}

/* 插入：保持按主键有序，使用插入排序位置 */
int darray_insert(DynamicArray* arr, const Record* rec) {
    int pos;
    int i;

    if (!arr || !rec) return ERROR;

    /* 二分查找插入位置 */
    {
        int low = 0;
        int high = arr->size - 1;
        int cmp;
        pos = 0;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            cmp = record_key_compare(rec, &arr->data[mid]);
            if (cmp == 0) return DUPLICATE;
            else if (cmp < 0) { pos = mid; high = mid - 1; }
            else { pos = mid + 1; low = mid + 1; }
        }
    }

    /* 容量不足则扩容 */
    if (arr->size >= arr->capacity) {
        if (darray_grow(arr) != OK) return ERROR;
    }

    /* 将 pos 之后的元素后移一位 */
    for (i = arr->size; i > pos; i--) {
        arr->data[i] = arr->data[i - 1];
    }

    arr->data[pos] = *rec;
    arr->size++;
    return OK;
}

/* 删除 */
int darray_delete(DynamicArray* arr, const char* sid, const char* cid) {
    int idx;
    int i;

    if (!arr || !sid || !cid) return ERROR;

    idx = binary_search(arr, sid, cid);
    if (idx < 0) return NOT_FOUND;

    /* 将 idx 之后的元素前移一位 */
    for (i = idx; i < arr->size - 1; i++) {
        arr->data[i] = arr->data[i + 1];
    }
    arr->size--;
    return OK;
}

/* 修改成绩 */
int darray_update(DynamicArray* arr, const char* sid, const char* cid, int new_score) {
    int idx;
    if (!arr || !sid || !cid) return ERROR;
    idx = binary_search(arr, sid, cid);
    if (idx < 0) return NOT_FOUND;
    arr->data[idx].score = new_score;
    return OK;
}

/* 查找 */
Record* darray_find(DynamicArray* arr, const char* sid, const char* cid) {
    int idx;
    if (!arr || !sid || !cid) return NULL;
    idx = binary_search(arr, sid, cid);
    if (idx < 0) return NULL;
    return &arr->data[idx];
}

/* 遍历（数组天然有序，直接顺序遍历即为有序输出） */
void darray_traverse(DynamicArray* arr, void (*visit)(const Record*)) {
    int i;
    if (!arr || !visit) return;
    for (i = 0; i < arr->size; i++) {
        visit(&arr->data[i]);
    }
}

int darray_size(DynamicArray* arr) {
    return arr ? arr->size : 0;
}
