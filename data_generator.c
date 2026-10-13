#include "data_generator.h"
#include <math.h>

/* 预设数据池：姓氏、名字改为字符串数组 */
static const char* surnames[] = {
    "赵","钱","孙","李","周","吴","郑","王",
    "冯","陈","褚","卫","蒋","沈","韩","杨",
    "朱","秦","尤","许","何","吕","施","张"
};
/* 自动计算姓氏总数 */
static const int surname_count = sizeof(surnames) / sizeof(surnames[0]);

static const char* names[] = {
    "伟","芳","娜","敏","静","丽","强","磊",
    "军","洋","勇","艳","杰","娟","涛","明",
    "超","秀","兰","霞","平","刚","桂","英"
};
/* 自动计算名字总数 */
static const int name_count = sizeof(names) / sizeof(names[0]);

static const char* colleges[] = {
    "计算机科学与工程学院", "数学与统计学院", "电子信息工程学院",
    "机械工程学院", "经济管理学院", "外国语学院", "土木工程学院"
};
static const int college_count = 7;

static const char* course_ids[] = {
    "CS300102", "CS300201", "CS300305", "MA200103",
    "EE200207", "ME100509", "EC300104", "FL200308"
};
static const char* course_names[] = {
    "数据结构与算法", "操作系统原理", "数据库系统原理", "高等数学",
    "模拟电子技术", "机械设计基础", "微观经济学", "大学英语"
};
static const float credits[] = {3.5f, 3.0f, 3.0f, 4.0f, 3.0f, 3.5f, 2.5f, 2.0f};
static const int course_count = 8;

/* 生成随机整数 [min, max] */
static int rand_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

/* 生成正态分布成绩（均值75，标准差12） */
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

/* 生成随机日期 */
static void rand_date(char* buf, int year_min, int year_max) {
    int year, month, day;
    year = rand_int(year_min, year_max);
    month = rand_int(1, 12);
    day = rand_int(1, 28); /* 简化处理 */
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
        /* 姓名生成相关变量，声明在块开头 */
        int s_idx, n1_idx, n2_idx;

        /* 学号：4位年份 + 2位学院码 + 6位序号 */
        enroll_year = rand_int(2020, 2025);
        college_code = rand_int(1, college_count);
        sprintf(records[i].student_id, "%04d%02d%06d", enroll_year, college_code, seq++);

        /* ---------- 修改核心：姓名生成逻辑 ---------- */
        s_idx = rand_int(0, surname_count - 1);
        n1_idx = rand_int(0, name_count - 1);
        /* 先拼接姓氏 */
        strcpy(records[i].name, surnames[s_idx]);
        /* 随机生成单字名或双字名 */
        if (rand() % 2) {
            n2_idx = rand_int(0, name_count - 1);
            strcat(records[i].name, names[n1_idx]);
            strcat(records[i].name, names[n2_idx]);
        } else {
            strcat(records[i].name, names[n1_idx]);
        }

        /* 学院 */
        col_idx = college_code - 1;
        strcpy(records[i].college, colleges[col_idx]);

        /* 课程 */
        c_idx = rand_int(0, course_count - 1);
        strcpy(records[i].course_id, course_ids[c_idx]);
        strcpy(records[i].course_name, course_names[c_idx]);
        records[i].credit = credits[c_idx];

        /* 学期 */
        year = rand_int(2020, 2026);
        sem = rand_int(1, 2);
        sprintf(records[i].semester, "%04d-%02d", year, sem);

        /* 选课日期 */
        rand_date(records[i].select_date, 2020, 2026);

        /* 成绩 */
        records[i].score = rand_score();
    }

    *actual_count = count;
    return records;
}

void free_records(Record* records) {
    if (records) free(records);
}
