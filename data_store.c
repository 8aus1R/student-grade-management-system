#include "data_store.h"
#include "linked_list.h"
#include "avl_tree.h"
#include "hash_table.h"
#include "dynamic_array.h"

/* ====================================================================
 *  ȫ�������ı��������̰߳�ȫ��
 * ==================================================================== */

static LinkedList* g_collect_list  = NULL;
static Record*     g_collect_arr   = NULL;
static int         g_collect_idx   = 0;
static int         g_collect_limit = 0;

static const char* g_match_str1   = NULL;
static int         g_match_min    = 0;
static int         g_match_max    = 0;
static int         g_count_result = 0;

typedef struct {
    char sid[ID_LEN];
    char cid[COURSE_ID_LEN];
} KeyPair;

static KeyPair* g_keys = NULL;
static int      g_kidx = 0;
static int      g_kcap = 0;

/* ====================================================================
 *  ��ֵ�ռ�����
 * ==================================================================== */

static void key_collect(const Record* rec) {
    if (g_kidx >= g_kcap) {
        g_kcap = (g_kcap == 0) ? 256 : (g_kcap * 2);
        g_keys = (KeyPair*)realloc(g_keys, g_kcap * sizeof(KeyPair));
        if (!g_keys) return;
    }
    strcpy(g_keys[g_kidx].sid, rec->student_id);
    strcpy(g_keys[g_kidx].cid, rec->course_id);
    g_kidx++;
}

static int keys_delete_and_free(DataStore* store) {
    int i, deleted = 0;
    for (i = 0; i < g_kidx; i++) {
        if (ds_delete(store, g_keys[i].sid, g_keys[i].cid) == OK)
            deleted++;
    }
    if (g_keys) { free(g_keys); g_keys = NULL; }
    g_kidx = 0;
    g_kcap = 0;
    return deleted;
}

static void keys_cleanup(void) {
    if (g_keys) { free(g_keys); g_keys = NULL; }
    g_kidx = 0;
    g_kcap = 0;
}

/* ====================================================================
 *  �����ص�����
 * ==================================================================== */

static void cb_collect_to_list(const Record* rec) {
    if (g_collect_list) list_insert(g_collect_list, rec);
}

static void cb_collect_to_array(const Record* rec) {
    if (g_collect_arr && g_collect_idx < g_collect_limit)
        g_collect_arr[g_collect_idx++] = *rec;
}

static void cb_count_by_student(const Record* rec) {
    if (strcmp(rec->student_id, g_match_str1) == 0) g_count_result++;
}

static void cb_count_by_course(const Record* rec) {
    if (strcmp(rec->course_id, g_match_str1) == 0) g_count_result++;
}

static void cb_count_by_college(const Record* rec) {
    if (strcmp(rec->college, g_match_str1) == 0) g_count_result++;
}

static void cb_count_by_semester(const Record* rec) {
    if (strcmp(rec->semester, g_match_str1) == 0) g_count_result++;
}

static void cb_count_by_score(const Record* rec) {
    if (rec->score >= g_match_min && rec->score <= g_match_max)
        g_count_result++;
}

static void cb_count_expired(const Record* rec) {
    if (date_compare(rec->select_date, g_match_str1) < 0)
        g_count_result++;
}

static void cb_collect_by_student(const Record* rec) {
    if (strcmp(rec->student_id, g_match_str1) == 0) key_collect(rec);
}

static void cb_collect_by_course(const Record* rec) {
    if (strcmp(rec->course_id, g_match_str1) == 0) key_collect(rec);
}

static void cb_collect_by_college(const Record* rec) {
    if (strcmp(rec->college, g_match_str1) == 0) key_collect(rec);
}

static void cb_collect_by_semester(const Record* rec) {
    if (strcmp(rec->semester, g_match_str1) == 0) key_collect(rec);
}

static void cb_collect_by_score(const Record* rec) {
    if (rec->score >= g_match_min && rec->score <= g_match_max)
        key_collect(rec);
}

static void cb_collect_expired(const Record* rec) {
    if (date_compare(rec->select_date, g_match_str1) < 0)
        key_collect(rec);
}

/* ====================================================================
 *  ���ݽṹ������ �� ����
 * ==================================================================== */

static int list_ad_insert(void* ds, const Record* r) { return list_insert((LinkedList*)ds, r); }
static int list_ad_delete(void* ds, const char* s, const char* c) { return list_delete((LinkedList*)ds, s, c); }
static int list_ad_update(void* ds, const char* s, const char* c, int n) { return list_update((LinkedList*)ds, s, c, n); }
static Record* list_ad_find(void* ds, const char* s, const char* c) { return list_find((LinkedList*)ds, s, c); }
static void list_ad_traverse(void* ds, void (*v)(const Record*)) { list_traverse((LinkedList*)ds, v); }
static void list_ad_destroy(void* ds) { list_destroy((LinkedList*)ds); }

/* ====================================================================
 *  ���ݽṹ������ �� AVL��
 * ==================================================================== */

static int avl_ad_insert(void* ds, const Record* r) { return avl_insert((AvlTree*)ds, r); }
static int avl_ad_delete(void* ds, const char* s, const char* c) { return avl_delete((AvlTree*)ds, s, c); }
static int avl_ad_update(void* ds, const char* s, const char* c, int n) { return avl_update((AvlTree*)ds, s, c, n); }
static Record* avl_ad_find(void* ds, const char* s, const char* c) { return avl_find((AvlTree*)ds, s, c); }
static void avl_ad_traverse(void* ds, void (*v)(const Record*)) { avl_traverse((AvlTree*)ds, v); }
static void avl_ad_destroy(void* ds) { avl_destroy((AvlTree*)ds); }

/* ====================================================================
 *  ���ݽṹ������ �� ��ϣ��
 * ==================================================================== */

static int hash_ad_insert(void* ds, const Record* r) { return hash_insert((HashTable*)ds, r); }
static int hash_ad_delete(void* ds, const char* s, const char* c) { return hash_delete((HashTable*)ds, s, c); }
static int hash_ad_update(void* ds, const char* s, const char* c, int n) { return hash_update((HashTable*)ds, s, c, n); }
static Record* hash_ad_find(void* ds, const char* s, const char* c) { return hash_find((HashTable*)ds, s, c); }
static void hash_ad_traverse(void* ds, void (*v)(const Record*)) { hash_traverse((HashTable*)ds, v); }
static void hash_ad_destroy(void* ds) { hash_destroy((HashTable*)ds); }

/* ====================================================================
 *  ���ݽṹ������ �� ��̬����
 * ==================================================================== */

static int darr_ad_insert(void* ds, const Record* r) { return darray_insert((DynamicArray*)ds, r); }
static int darr_ad_delete(void* ds, const char* s, const char* c) { return darray_delete((DynamicArray*)ds, s, c); }
static int darr_ad_update(void* ds, const char* s, const char* c, int n) { return darray_update((DynamicArray*)ds, s, c, n); }
static Record* darr_ad_find(void* ds, const char* s, const char* c) { return darray_find((DynamicArray*)ds, s, c); }
static void darr_ad_traverse(void* ds, void (*v)(const Record*)) { darray_traverse((DynamicArray*)ds, v); }
static void darr_ad_destroy(void* ds) { darray_destroy((DynamicArray*)ds); }

/* ====================================================================
 *  �󶨺���ָ���
 * ==================================================================== */

static void bind_list(DataStore* st) {
    st->type = STORE_LINKED_LIST;
    st->insert = list_ad_insert; st->del = list_ad_delete;
    st->update = list_ad_update; st->find = list_ad_find;
    st->traverse = list_ad_traverse; st->destroy = list_ad_destroy;
}

static void bind_avl(DataStore* st) {
    st->type = STORE_AVL_TREE;
    st->insert = avl_ad_insert; st->del = avl_ad_delete;
    st->update = avl_ad_update; st->find = avl_ad_find;
    st->traverse = avl_ad_traverse; st->destroy = avl_ad_destroy;
}

static void bind_hash(DataStore* st) {
    st->type = STORE_HASH_TABLE;
    st->insert = hash_ad_insert; st->del = hash_ad_delete;
    st->update = hash_ad_update; st->find = hash_ad_find;
    st->traverse = hash_ad_traverse; st->destroy = hash_ad_destroy;
}

static void bind_darr(DataStore* st) {
    st->type = STORE_DYNAMIC_ARRAY;
    st->insert = darr_ad_insert; st->del = darr_ad_delete;
    st->update = darr_ad_update; st->find = darr_ad_find;
    st->traverse = darr_ad_traverse; st->destroy = darr_ad_destroy;
}

/* ====================================================================
 *  ���� API �� ��������
 * ==================================================================== */

DataStore* ds_create(enum StoreType type) {
    DataStore* s = (DataStore*)malloc(sizeof(DataStore));
    if (!s) return NULL;
    s->count = 0;
    switch (type) {
        case STORE_LINKED_LIST:   s->structure = list_create();   bind_list(s); break;
        case STORE_AVL_TREE:      s->structure = avl_create();    bind_avl(s);  break;
        case STORE_HASH_TABLE:    s->structure = hash_create();   bind_hash(s); break;
        case STORE_DYNAMIC_ARRAY: s->structure = darray_create(); bind_darr(s); break;
        default: free(s); return NULL;
    }
    if (!s->structure) { free(s); return NULL; }
    return s;
}

void ds_destroy(DataStore* store) {
    if (!store) return;
    if (store->structure) store->destroy(store->structure);
    free(store);
}

int ds_insert(DataStore* store, const Record* rec) {
    int ret;
    if (!store || !rec) return ERROR;
    ret = store->insert(store->structure, rec);
    if (ret == OK) store->count++;
    return ret;
}

int ds_delete(DataStore* store, const char* sid, const char* cid) {
    int ret;
    if (!store || !sid || !cid) return ERROR;
    ret = store->del(store->structure, sid, cid);
    if (ret == OK) store->count--;
    return ret;
}

int ds_update(DataStore* store, const char* sid, const char* cid, int new_score) {
    if (!store || !sid || !cid) return ERROR;
    return store->update(store->structure, sid, cid, new_score);
}

Record* ds_find(DataStore* store, const char* sid, const char* cid) {
    if (!store || !sid || !cid) return NULL;
    return store->find(store->structure, sid, cid);
}

void ds_traverse(DataStore* store, void (*visit)(const Record*)) {
    if (!store || !visit) return;
    store->traverse(store->structure, visit);
}

int ds_size(DataStore* store) {
    return store ? store->count : 0;
}

const char* ds_type_name(DataStore* store) {
    if (!store) return "δ֪";
    switch (store->type) {
        case STORE_LINKED_LIST:   return "����";
        case STORE_AVL_TREE:      return "AVL��";
        case STORE_HASH_TABLE:    return "��ϣ��";
        case STORE_DYNAMIC_ARRAY: return "��̬����";
        default: return "δ֪";
    }
}

/* ====================================================================
 *  ���� API �� ����
 * ==================================================================== */

LinkedList* ds_export_to_list(DataStore* store) {
    LinkedList* result;
    if (!store) return NULL;
    result = list_create();
    if (!result) return NULL;
    g_collect_list = result;
    store->traverse(store->structure, cb_collect_to_list);
    g_collect_list = NULL;
    return result;
}

Record* ds_export_array(DataStore* store, int* out_count) {
    Record* arr;
    if (!store || !out_count) return NULL;
    if (store->count <= 0) { *out_count = 0; return NULL; }
    arr = (Record*)malloc(store->count * sizeof(Record));
    if (!arr) { *out_count = 0; return NULL; }
    g_collect_arr = arr;
    g_collect_limit = store->count;
    g_collect_idx = 0;
    store->traverse(store->structure, cb_collect_to_array);
    *out_count = g_collect_idx;
    g_collect_arr = NULL;
    return arr;
}

/* ====================================================================
 *  ���� API �� �л����ݽṹ
 * ==================================================================== */

int ds_switch_type(DataStore* store, enum StoreType new_type) {
    LinkedList* temp;
    void* old;
    ListNode* cur;

    if (!store) return ERROR;
    if (store->type == new_type) return OK;

    temp = list_create();
    if (!temp) return ERROR;
    g_collect_list = temp;
    store->traverse(store->structure, cb_collect_to_list);
    g_collect_list = NULL;

    old = store->structure;
    store->destroy(old);

    switch (new_type) {
        case STORE_LINKED_LIST:
            store->structure = temp;
            bind_list(store);
            break;
        case STORE_AVL_TREE: {
            AvlTree* tree = avl_create();
            if (!tree) { list_destroy(temp); return ERROR; }
            cur = temp->head;
            while (cur) { avl_insert(tree, &cur->data); cur = cur->next; }
            list_destroy(temp);
            store->structure = tree;
            bind_avl(store);
            break;
        }
        case STORE_HASH_TABLE: {
            HashTable* table = hash_create();
            if (!table) { list_destroy(temp); return ERROR; }
            cur = temp->head;
            while (cur) { hash_insert(table, &cur->data); cur = cur->next; }
            list_destroy(temp);
            store->structure = table;
            bind_hash(store);
            break;
        }
        case STORE_DYNAMIC_ARRAY: {
            DynamicArray* darr = darray_create();
            if (!darr) { list_destroy(temp); return ERROR; }
            cur = temp->head;
            while (cur) { darray_insert(darr, &cur->data); cur = cur->next; }
            list_destroy(temp);
            store->structure = darr;
            bind_darr(store);
            break;
        }
        default:
            list_destroy(temp);
            return ERROR;
    }
    return OK;
}

/* ====================================================================
 *  ���� API �� ��������ɾ��������
 * ==================================================================== */

int ds_count_by_student(DataStore* store, const char* sid) {
    if (!store || !sid) return 0;
    g_match_str1 = sid; g_count_result = 0;
    store->traverse(store->structure, cb_count_by_student);
    g_match_str1 = NULL;
    return g_count_result;
}

int ds_count_by_course(DataStore* store, const char* cid) {
    if (!store || !cid) return 0;
    g_match_str1 = cid; g_count_result = 0;
    store->traverse(store->structure, cb_count_by_course);
    g_match_str1 = NULL;
    return g_count_result;
}

int ds_count_by_college(DataStore* store, const char* college) {
    if (!store || !college) return 0;
    g_match_str1 = college; g_count_result = 0;
    store->traverse(store->structure, cb_count_by_college);
    g_match_str1 = NULL;
    return g_count_result;
}

int ds_count_by_semester(DataStore* store, const char* semester) {
    if (!store || !semester) return 0;
    g_match_str1 = semester; g_count_result = 0;
    store->traverse(store->structure, cb_count_by_semester);
    g_match_str1 = NULL;
    return g_count_result;
}

int ds_count_by_score(DataStore* store, int min_score, int max_score) {
    if (!store) return 0;
    g_match_min = min_score; g_match_max = max_score; g_count_result = 0;
    store->traverse(store->structure, cb_count_by_score);
    return g_count_result;
}

int ds_count_expired(DataStore* store, const char* base_date, int years) {
    int y, m, d;
    char cutoff[DATE_LEN];
    if (!store || !base_date || !is_valid_date(base_date)) return 0;
    sscanf(base_date, "%d-%d-%d", &y, &m, &d);
    sprintf(cutoff, "%04d-%02d-%02d", y - years, m, d);
    g_match_str1 = cutoff; g_count_result = 0;
    store->traverse(store->structure, cb_count_expired);
    g_match_str1 = NULL;
    return g_count_result;
}

/* ====================================================================
 *  ���� API �� ��������ɾ����ִ��
 * ==================================================================== */

int ds_delete_by_student(DataStore* store, const char* sid) {
    if (!store || !sid) return 0;
    g_match_str1 = sid; keys_cleanup();
    store->traverse(store->structure, cb_collect_by_student);
    g_match_str1 = NULL;
    return keys_delete_and_free(store);
}

int ds_delete_by_course(DataStore* store, const char* cid) {
    if (!store || !cid) return 0;
    g_match_str1 = cid; keys_cleanup();
    store->traverse(store->structure, cb_collect_by_course);
    g_match_str1 = NULL;
    return keys_delete_and_free(store);
}

int ds_delete_by_college(DataStore* store, const char* college) {
    if (!store || !college) return 0;
    g_match_str1 = college; keys_cleanup();
    store->traverse(store->structure, cb_collect_by_college);
    g_match_str1 = NULL;
    return keys_delete_and_free(store);
}

int ds_delete_by_semester(DataStore* store, const char* semester) {
    if (!store || !semester) return 0;
    g_match_str1 = semester; keys_cleanup();
    store->traverse(store->structure, cb_collect_by_semester);
    g_match_str1 = NULL;
    return keys_delete_and_free(store);
}

int ds_delete_by_score(DataStore* store, int min_score, int max_score) {
    if (!store) return 0;
    g_match_min = min_score; g_match_max = max_score;
    keys_cleanup();
    store->traverse(store->structure, cb_collect_by_score);
    return keys_delete_and_free(store);
}

int ds_delete_expired(DataStore* store, const char* base_date, int years) {
    int y, m, d;
    char cutoff[DATE_LEN];
    if (!store || !base_date || !is_valid_date(base_date)) return 0;
    sscanf(base_date, "%d-%d-%d", &y, &m, &d);
    sprintf(cutoff, "%04d-%02d-%02d", y - years, m, d);
    g_match_str1 = cutoff; keys_cleanup();
    store->traverse(store->structure, cb_collect_expired);
    g_match_str1 = NULL;
    return keys_delete_and_free(store);
}
