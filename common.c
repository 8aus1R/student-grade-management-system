#include "common.h"

/* 日期比较：date1 < date2 返回-1，相等返回0，大于返回1 */
int date_compare(const char* date1, const char* date2) {
    int y1, m1, d1, y2, m2, d2;
    sscanf(date1, "%d-%d-%d", &y1, &m1, &d1);
    sscanf(date2, "%d-%d-%d", &y2, &m2, &d2);

    if (y1 != y2) return y1 < y2 ? -1 : 1;
    if (m1 != m2) return m1 < m2 ? -1 : 1;
    if (d1 != d2) return d1 < d2 ? -1 : 1;
    return 0;
}

/* 模糊字符串匹配 */
bool fuzzy_match(const char* source, const char* pattern) {
    return strstr(source, pattern) != NULL;
}

/* 打印单条记录 */
void print_record(const Record* rec) {
    printf("%-12s %-8s %-20s %-8s %-20s %4.1f %-7s %-10s %3d\n",
           rec->student_id, rec->name, rec->college,
           rec->course_id, rec->course_name, rec->credit,
           rec->semester, rec->select_date, rec->score);
}

/* 记录主键比较（学号+课程号），用于排序/查找 */
int record_key_compare(const Record* a, const Record* b) {
    int cmp = strcmp(a->student_id, b->student_id);
    if (cmp != 0) return cmp;
    return strcmp(a->course_id, b->course_id);
}

/* 生成主键字符串（学号+课程号） */
char* get_record_key(const Record* rec, char* key_buf) {
    sprintf(key_buf, "%s%s", rec->student_id, rec->course_id);
    return key_buf;
}

/* 校验日期格式与合法性（含闰年处理） */
bool is_valid_date(const char* date) {
    int y, m, d;
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (strlen(date) != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    if (sscanf(date, "%d-%d-%d", &y, &m, &d) != 3) return false;
    if (y < 2000 || y > 2100) return false;
    if (m < 1 || m > 12) return false;
    if (m == 2 && (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0)))
        days_in_month[1] = 29;
    if (d < 1 || d > days_in_month[m - 1]) return false;
    return true;
}

/* 打印当前记录详情 */
static void print_record_detail(const Record* rec) {
    printf("\n=== 当前记录详情 ===\n");
    printf("学号:       %s\n", rec->student_id);
    printf("姓名:       %s\n", rec->name);
    printf("学院:       %s\n", rec->college);
    printf("课程编号:   %s\n", rec->course_id);
    printf("课程名称:   %s\n", rec->course_name);
    printf("学分:       %.1f\n", rec->credit);
    printf("选课学期:   %s\n", rec->semester);
    printf("选课日期:   %s\n", rec->select_date);
    printf("成绩:       %d\n", rec->score);
    printf("====================\n");
}

/* 交互式修改记录字段 */
bool modify_record_interactive(Record* rec) {
    int choice;
    bool modified = false;
    char input_buf[MAX_STR_LEN];
    int new_score;

    if (!rec) return false;

    print_record_detail(rec);

    while (1) {
        printf("\n--- 修改字段 ---\n");
        printf("1. 成绩\n");
        printf("2. 选课学期\n");
        printf("3. 选课日期\n");
        printf("4. 姓名\n");
        printf("5. 学院\n");
        printf("6. 课程名称\n");
        printf("0. 完成修改\n");
        printf("请选择要修改的字段: ");
        scanf("%d", &choice);

        switch (choice) {
            case MODIFY_SCORE:
                printf("当前成绩: %d\n", rec->score);
                printf("请输入新成绩(0-100): ");
                scanf("%d", &new_score);
                if (new_score < 0 || new_score > 100) {
                    printf("成绩超出范围，修改失败\n");
                } else {
                    rec->score = new_score;
                    printf("成绩已更新为 %d\n", rec->score);
                    modified = true;
                }
                break;

            case MODIFY_SEMESTER:
                printf("当前学期: %s\n", rec->semester);
                printf("请输入新学期(格式 YYYY-XX，如 2025-01): ");
                scanf("%s", input_buf);
                if (strlen(input_buf) != 7 || input_buf[4] != '-') {
                    printf("学期格式错误，修改失败\n");
                } else {
                    strncpy(rec->semester, input_buf, SEMESTER_LEN - 1);
                    rec->semester[SEMESTER_LEN - 1] = '\0';
                    printf("学期已更新为 %s\n", rec->semester);
                    modified = true;
                }
                break;

            case MODIFY_SELECT_DATE:
                printf("当前选课日期: %s\n", rec->select_date);
                printf("请输入新日期(格式 YYYY-MM-DD): ");
                scanf("%s", input_buf);
                if (!is_valid_date(input_buf)) {
                    printf("日期格式错误，修改失败\n");
                } else {
                    strncpy(rec->select_date, input_buf, DATE_LEN - 1);
                    rec->select_date[DATE_LEN - 1] = '\0';
                    printf("选课日期已更新为 %s\n", rec->select_date);
                    modified = true;
                }
                break;

            case MODIFY_NAME:
                printf("当前姓名: %s\n", rec->name);
                printf("请输入新姓名: ");
                scanf("%s", input_buf);
                if (strlen(input_buf) == 0) {
                    printf("姓名不能为空，修改失败\n");
                } else {
                    strncpy(rec->name, input_buf, MAX_STR_LEN - 1);
                    rec->name[MAX_STR_LEN - 1] = '\0';
                    printf("姓名已更新为 %s\n", rec->name);
                    modified = true;
                }
                break;

            case MODIFY_COLLEGE:
                printf("当前学院: %s\n", rec->college);
                printf("请输入新学院: ");
                scanf("%s", input_buf);
                if (strlen(input_buf) == 0) {
                    printf("学院不能为空，修改失败\n");
                } else {
                    strncpy(rec->college, input_buf, MAX_STR_LEN - 1);
                    rec->college[MAX_STR_LEN - 1] = '\0';
                    printf("学院已更新为 %s\n", rec->college);
                    modified = true;
                }
                break;

            case MODIFY_COURSE_NAME:
                printf("当前课程名称: %s\n", rec->course_name);
                printf("请输入新课程名称: ");
                scanf("%s", input_buf);
                if (strlen(input_buf) == 0) {
                    printf("课程名称不能为空，修改失败\n");
                } else {
                    strncpy(rec->course_name, input_buf, MAX_STR_LEN - 1);
                    rec->course_name[MAX_STR_LEN - 1] = '\0';
                    printf("课程名称已更新为 %s\n", rec->course_name);
                    modified = true;
                }
                break;

            case 0:
                if (modified) {
                    printf("\n--- 修改后记录详情 ---\n");
                    print_record_detail(rec);
                }
                return modified;

            default:
                printf("无效选项\n");
        }
    }
}
