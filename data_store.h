#ifndef DATA_STORE_H
#define DATA_STORE_H

#include "common.h"
#include "linked_list.h"

enum StoreType {
    STORE_LINKED_LIST   = 1,
    STORE_AVL_TREE      = 2,
    STORE_HASH_TABLE    = 3,
    STORE_DYNAMIC_ARRAY = 4
};

typedef struct DataStore {
    enum StoreType type;
    void* structure;
    int count;

    int     (*insert)(void* ds, const Record* rec);
    int     (*del)(void* ds, const char* sid, const char* cid);
    int     (*update)(void* ds, const char* sid, const char* cid, int new_score);
    Record* (*find)(void* ds, const char* sid, const char* cid);
    void    (*traverse)(void* ds, void (*visit)(const Record*));
    void    (*destroy)(void* ds);
} DataStore;

DataStore* ds_create(enum StoreType type);
void       ds_destroy(DataStore* store);

int      ds_insert(DataStore* store, const Record* rec);
int      ds_delete(DataStore* store, const char* sid, const char* cid);
int      ds_update(DataStore* store, const char* sid, const char* cid, int new_score);
Record*  ds_find(DataStore* store, const char* sid, const char* cid);
void     ds_traverse(DataStore* store, void (*visit)(const Record*));
int      ds_size(DataStore* store);

const char* ds_type_name(DataStore* store);
int         ds_switch_type(DataStore* store, enum StoreType new_type);

Record*     ds_export_array(DataStore* store, int* out_count);
LinkedList* ds_export_to_list(DataStore* store);

int ds_count_by_student  (DataStore* store, const char* sid);
int ds_count_by_course   (DataStore* store, const char* cid);
int ds_count_by_college  (DataStore* store, const char* college);
int ds_count_by_semester (DataStore* store, const char* semester);
int ds_count_by_score    (DataStore* store, int min_score, int max_score);
int ds_count_expired     (DataStore* store, const char* base_date, int years);

int ds_delete_by_student  (DataStore* store, const char* sid);
int ds_delete_by_course   (DataStore* store, const char* cid);
int ds_delete_by_college  (DataStore* store, const char* college);
int ds_delete_by_semester (DataStore* store, const char* semester);
int ds_delete_by_score    (DataStore* store, int min_score, int max_score);
int ds_delete_expired     (DataStore* store, const char* base_date, int years);

#endif
