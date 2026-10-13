#ifndef QUERY_H
#define QUERY_H

#include "common.h"
#include "linked_list.h"

/* 筛选条件结构体 */
typedef struct {
    bool use_course_name;
    char course_name[MAX_STR_LEN];
    bool fuzzy_course;

    bool use_semester;
    char semester[SEMESTER_LEN];

    bool use_score_range;
    int score_min;
    int score_max;

    bool use_college;
    char college[MAX_STR_LEN];
} FilterParams;

/* 排序键 */
typedef struct {
    char field[MAX_STR_LEN]; /* 字段名：score/student_id/course_id等 */
    bool descending;
} SortKey;

/* 多条件筛选，返回结果链表 */
LinkedList* filter_records(LinkedList* source, const FilterParams* params);

/* 多关键字排序（结果导出到数组后快排） */
void sort_records(Record* arr, int len, SortKey* keys, int key_count);

/* 统计：各课程选课人数 */
void stat_course_enrollment(LinkedList* list);

/* 统计：成绩分布 */
void stat_score_distribution(LinkedList* list);

#endif
