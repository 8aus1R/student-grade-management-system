#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define true  1
#define false 0

#define MAX_STR_LEN 128
#define ID_LEN 13       /* 12λѧ��+������ */
#define COURSE_ID_LEN 9 /* 8λ�γ̺�+������ */
#define DATE_LEN 11     /* YYYY-MM-DD */
#define SEMESTER_LEN 8  /* YYYY-XX */

/* ѡ�μ�¼�ṹ�� */
typedef struct {
    char student_id[ID_LEN];
    char name[MAX_STR_LEN];
    char college[MAX_STR_LEN];
    char course_id[COURSE_ID_LEN];
    char course_name[MAX_STR_LEN];
    float credit;
    char semester[SEMESTER_LEN];
    char select_date[DATE_LEN];
    int score;
} Record;

/* ״̬�� */
enum Status {
    OK = 0,
    ERROR = -1,
    NOT_FOUND = -2,
    DUPLICATE = -3
};

/* �޸��ֶ�ѡ�� */
enum ModifyField {
    MODIFY_SCORE = 1,
    MODIFY_SEMESTER = 2,
    MODIFY_SELECT_DATE = 3,
    MODIFY_NAME = 4,
    MODIFY_COLLEGE = 5,
    MODIFY_COURSE_NAME = 6
};

/* ���ߺ������� */
int date_compare(const char* date1, const char* date2);
bool fuzzy_match(const char* source, const char* pattern);
void print_record(const Record* rec);
int record_key_compare(const Record* a, const Record* b);
char* get_record_key(const Record* rec, char* key_buf);

/* У�����ڸ�ʽ�Ƿ�Ϸ���YYYY-MM-DD�� */
bool is_valid_date(const char* date);

/* ����ʽ�޸ļ�¼�ֶ� */
bool modify_record_interactive(Record* rec);

#endif
