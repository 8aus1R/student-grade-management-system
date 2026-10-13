#ifndef QUERY_H
#define QUERY_H

#include "common.h"
#include "linked_list.h"

/* ɸѡ�����ṹ�� */
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

/* ����� */
typedef struct {
    char field[MAX_STR_LEN]; /* �ֶ�����score/student_id/course_id�� */
    bool descending;
} SortKey;

/* ������ɸѡ�����ؽ������ */
LinkedList* filter_records(LinkedList* source, const FilterParams* params);

/* ��ؼ������򣨽���������������ţ� */
void sort_records(Record* arr, int len, SortKey* keys, int key_count);

/* ͳ�ƣ����γ�ѡ������ */
void stat_course_enrollment(LinkedList* list);

/* ͳ�ƣ��ɼ��ֲ� */
void stat_score_distribution(LinkedList* list);

#endif
