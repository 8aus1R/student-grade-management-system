#include "query.h"

/* 检查单条记录是否符合筛选条件 */
static bool match_filter(const Record* rec, const FilterParams* params) {
    if (params->use_course_name) {
        if (params->fuzzy_course) {
            if (!fuzzy_match(rec->course_name, params->course_name)) return false;
        } else {
            if (strcmp(rec->course_name, params->course_name) != 0) return false;
        }
    }

    if (params->use_semester) {
        if (strcmp(rec->semester, params->semester) != 0) return false;
    }

    if (params->use_score_range) {
        if (rec->score < params->score_min || rec->score > params->score_max) return false;
    }

    if (params->use_college) {
        if (strcmp(rec->college, params->college) != 0) return false;
    }

    return true;
}

LinkedList* filter_records(LinkedList* source, const FilterParams* params) {
    LinkedList* result;
    ListNode* cur;

    if (!source || !params) return NULL;
    result = list_create();
    if (!result) return NULL;

    cur = source->head;
    while (cur) {
        if (match_filter(&cur->data, params)) {
            list_insert(result, &cur->data);
        }
        cur = cur->next;
    }
    return result;
}

/* 多字段比较函数（供快排使用） */
static int compare_multi(const void* a, const void* b, SortKey* keys, int key_count) {
    const Record* ra = (const Record*)a;
    const Record* rb = (const Record*)b;
    int i;

    for (i = 0; i < key_count; i++) {
        int cmp = 0;
        if (strcmp(keys[i].field, "score") == 0) {
            cmp = ra->score - rb->score;
        } else if (strcmp(keys[i].field, "student_id") == 0) {
            cmp = strcmp(ra->student_id, rb->student_id);
        } else if (strcmp(keys[i].field, "course_id") == 0) {
            cmp = strcmp(ra->course_id, rb->course_id);
        }

        if (cmp != 0) {
            return keys[i].descending ? -cmp : cmp;
        }
    }
    return 0;
}

/* 简易快排实现（C89 qsort无法传参，手动实现） */
static void quick_sort(Record* arr, int left, int right, SortKey* keys, int key_count) {
    int i, j;
    Record pivot;

    if (left >= right) return;

    pivot = arr[left];
    i = left;
    j = right;

    while (i < j) {
        while (i < j && compare_multi(&arr[j], &pivot, keys, key_count) >= 0) j--;
        arr[i] = arr[j];
        while (i < j && compare_multi(&arr[i], &pivot, keys, key_count) <= 0) i++;
        arr[j] = arr[i];
    }
    arr[i] = pivot;

    quick_sort(arr, left, i-1, keys, key_count);
    quick_sort(arr, i+1, right, keys, key_count);
}

void sort_records(Record* arr, int len, SortKey* keys, int key_count) {
    if (!arr || len <= 1 || !keys || key_count <= 0) return;
    quick_sort(arr, 0, len-1, keys, key_count);
}

/* 统计课程选课人数（简易实现，用数组遍历） */
void stat_course_enrollment(LinkedList* list) {
    /* 预设课程统计 */
    typedef struct {
        char course_id[COURSE_ID_LEN];
        char course_name[MAX_STR_LEN];
        int count;
    } CourseStat;

    CourseStat stats[20];
    int stat_count = 0;
    ListNode* cur;
    int i;

    if (!list) return;

    cur = list->head;
    while (cur) {
        int found = 0;
        for (i = 0; i < stat_count; i++) {
            if (strcmp(stats[i].course_id, cur->data.course_id) == 0) {
                stats[i].count++;
                found = 1;
                break;
            }
        }
        if (!found && stat_count < 20) {
            strcpy(stats[stat_count].course_id, cur->data.course_id);
            strcpy(stats[stat_count].course_name, cur->data.course_name);
            stats[stat_count].count = 1;
            stat_count++;
        }
        cur = cur->next;
    }

    printf("\n%-10s %-20s %-8s\n", "课程编号", "课程名称", "选课人数");
    printf("----------------------------------------\n");
    for (i = 0; i < stat_count; i++) {
        printf("%-10s %-20s %-8d\n", stats[i].course_id, stats[i].course_name, stats[i].count);
    }
}

/* 统计成绩分布 */
void stat_score_distribution(LinkedList* list) {
    int excellent = 0, good = 0, medium = 0, pass = 0, fail = 0;
    ListNode* cur;

    if (!list) return;

    cur = list->head;
    while (cur) {
        int s = cur->data.score;
        if (s >= 90) excellent++;
        else if (s >= 80) good++;
        else if (s >= 70) medium++;
        else if (s >= 60) pass++;
        else fail++;
        cur = cur->next;
    }

    printf("\n成绩分布统计:\n");
    printf("优秀(90-100): %d人\n", excellent);
    printf("良好(80-89):  %d人\n", good);
    printf("中等(70-79):  %d人\n", medium);
    printf("及格(60-69):  %d人\n", pass);
    printf("不及格(<60):  %d人\n", fail);
}
