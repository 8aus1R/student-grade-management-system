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

/* ���ֲ��ң�����ƥ���¼��������δ�ҵ����� -1 */
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

/* ���룺���ְ���������ʹ�ò�������λ�� */
int darray_insert(DynamicArray* arr, const Record* rec) {
    int pos;
    int i;

    if (!arr || !rec) return ERROR;

    /* ���ֲ��Ҳ���λ�� */
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

    /* �������������� */
    if (arr->size >= arr->capacity) {
        if (darray_grow(arr) != OK) return ERROR;
    }

    /* �� pos ֮���Ԫ�غ���һλ */
    for (i = arr->size; i > pos; i--) {
        arr->data[i] = arr->data[i - 1];
    }

    arr->data[pos] = *rec;
    arr->size++;
    return OK;
}

/* ɾ�� */
int darray_delete(DynamicArray* arr, const char* sid, const char* cid) {
    int idx;
    int i;

    if (!arr || !sid || !cid) return ERROR;

    idx = binary_search(arr, sid, cid);
    if (idx < 0) return NOT_FOUND;

    /* �� idx ֮���Ԫ��ǰ��һλ */
    for (i = idx; i < arr->size - 1; i++) {
        arr->data[i] = arr->data[i + 1];
    }
    arr->size--;
    return OK;
}

/* �޸ĳɼ� */
int darray_update(DynamicArray* arr, const char* sid, const char* cid, int new_score) {
    int idx;
    if (!arr || !sid || !cid) return ERROR;
    idx = binary_search(arr, sid, cid);
    if (idx < 0) return NOT_FOUND;
    arr->data[idx].score = new_score;
    return OK;
}

/* ���� */
Record* darray_find(DynamicArray* arr, const char* sid, const char* cid) {
    int idx;
    if (!arr || !sid || !cid) return NULL;
    idx = binary_search(arr, sid, cid);
    if (idx < 0) return NULL;
    return &arr->data[idx];
}

/* ������������Ȼ����ֱ��˳�������Ϊ��������� */
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
