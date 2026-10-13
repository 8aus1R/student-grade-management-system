#include "data_generator.h"
#include <math.h>

/* Ԥ�����ݳأ����ϡ����ָ�Ϊ�ַ������� */
static const char* surnames[] = {
    "��","Ǯ","��","��","��","��","֣","��",
    "��","��","��","��","��","��","��","��",
    "��","��","��","��","��","��","ʩ","��"
};
/* �Զ������������� */
static const int surname_count = sizeof(surnames) / sizeof(surnames[0]);

static const char* names[] = {
    "ΰ","��","��","��","��","��","ǿ","��",
    "��","��","��","��","��","��","��","��",
    "��","��","��","ϼ","ƽ","��","��","Ӣ"
};
/* �Զ������������� */
static const int name_count = sizeof(names) / sizeof(names[0]);

static const char* colleges[] = {
    "�������ѧ�빤��ѧԺ", "��ѧ��ͳ��ѧԺ", "������Ϣ����ѧԺ",
    "��е����ѧԺ", "���ù���ѧԺ", "�����ѧԺ", "��ľ����ѧԺ"
};
static const int college_count = 7;

static const char* course_ids[] = {
    "CS300102", "CS300201", "CS300305", "MA200103",
    "EE200207", "ME100509", "EC300104", "FL200308"
};
static const char* course_names[] = {
    "���ݽṹ���㷨", "����ϵͳԭ��", "���ݿ�ϵͳԭ��", "�ߵ���ѧ",
    "ģ����Ӽ���", "��е��ƻ���", "΢�۾���ѧ", "��ѧӢ��"
};
static const float credits[] = {3.5f, 3.0f, 3.0f, 4.0f, 3.0f, 3.5f, 2.5f, 2.0f};
static const int course_count = 8;

/* ����������� [min, max] */
static int rand_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

/* ������̬�ֲ��ɼ�����ֵ75����׼��12�� */
static int rand_score() {
    double u1, u2, z;
    int score;
    u1 = (double)rand() / RAND_MAX;
    u2 = (double)rand() / RAND_MAX;
    z = sqrt(-2 * log(u1)) * cos(2 * 3.1415926 * u2);
    score = (int)(75 + 12 * z);
    if (score < 0) score = 0;
    if (score > 100) score = 100;
    return score;
}

/* ����������� */
static void rand_date(char* buf, int year_min, int year_max) {
    int year, month, day;
    year = rand_int(year_min, year_max);
    month = rand_int(1, 12);
    day = rand_int(1, 28); /* �򻯴��� */
    sprintf(buf, "%04d-%02d-%02d", year, month, day);
}

Record* generate_records(int count, int* actual_count) {
    Record* records;
    int seq;
    int i;

    if (count <= 0) {
        *actual_count = 0;
        return NULL;
    }

    records = (Record*)malloc(count * sizeof(Record));
    if (!records) {
        *actual_count = 0;
        return NULL;
    }

    srand((unsigned int)time(NULL));
    seq = 1;

    for (i = 0; i < count; i++) {
        int enroll_year, college_code;
        int col_idx, c_idx;
        int year, sem;
        /* ����������ر����������ڿ鿪ͷ */
        int s_idx, n1_idx, n2_idx;

        /* ѧ�ţ�4λ��� + 2λѧԺ�� + 6λ��� */
        enroll_year = rand_int(2020, 2025);
        college_code = rand_int(1, college_count);
        sprintf(records[i].student_id, "%04d%02d%06d", enroll_year, college_code, seq++);

        /* ---------- �޸ĺ��ģ����������߼� ---------- */
        s_idx = rand_int(0, surname_count - 1);
        n1_idx = rand_int(0, name_count - 1);
        /* ��ƴ������ */
        strcpy(records[i].name, surnames[s_idx]);
        /* ������ɵ�������˫���� */
        if (rand() % 2) {
            n2_idx = rand_int(0, name_count - 1);
            strcat(records[i].name, names[n1_idx]);
            strcat(records[i].name, names[n2_idx]);
        } else {
            strcat(records[i].name, names[n1_idx]);
        }

        /* ѧԺ */
        col_idx = college_code - 1;
        strcpy(records[i].college, colleges[col_idx]);

        /* �γ� */
        c_idx = rand_int(0, course_count - 1);
        strcpy(records[i].course_id, course_ids[c_idx]);
        strcpy(records[i].course_name, course_names[c_idx]);
        records[i].credit = credits[c_idx];

        /* ѧ�� */
        year = rand_int(2020, 2026);
        sem = rand_int(1, 2);
        sprintf(records[i].semester, "%04d-%02d", year, sem);

        /* ѡ������ */
        rand_date(records[i].select_date, 2020, 2026);

        /* �ɼ� */
        records[i].score = rand_score();
    }

    *actual_count = count;
    return records;
}

void free_records(Record* records) {
    if (records) free(records);
}
