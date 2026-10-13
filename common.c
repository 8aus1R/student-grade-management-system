#include "common.h"

/* ���ڱȽϣ�date1 < date2 ����-1����ȷ���0�����ڷ���1 */
int date_compare(const char* date1, const char* date2) {
    int y1, m1, d1, y2, m2, d2;
    sscanf(date1, "%d-%d-%d", &y1, &m1, &d1);
    sscanf(date2, "%d-%d-%d", &y2, &m2, &d2);

    if (y1 != y2) return y1 < y2 ? -1 : 1;
    if (m1 != m2) return m1 < m2 ? -1 : 1;
    if (d1 != d2) return d1 < d2 ? -1 : 1;
    return 0;
}

/* ģ���ַ���ƥ�� */
bool fuzzy_match(const char* source, const char* pattern) {
    return strstr(source, pattern) != NULL;
}

/* ��ӡ������¼ */
void print_record(const Record* rec) {
    printf("%-12s %-8s %-20s %-8s %-20s %4.1f %-7s %-10s %3d\n",
           rec->student_id, rec->name, rec->college,
           rec->course_id, rec->course_name, rec->credit,
           rec->semester, rec->select_date, rec->score);
}

/* ��¼�����Ƚϣ�ѧ��+�γ̺ţ�����������/���� */
int record_key_compare(const Record* a, const Record* b) {
    int cmp = strcmp(a->student_id, b->student_id);
    if (cmp != 0) return cmp;
    return strcmp(a->course_id, b->course_id);
}

/* ���������ַ�����ѧ��+�γ̺ţ� */
char* get_record_key(const Record* rec, char* key_buf) {
    sprintf(key_buf, "%s%s", rec->student_id, rec->course_id);
    return key_buf;
}

/* У�����ڸ�ʽ��Ϸ��ԣ������괦���� */
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

/* ��ӡ��ǰ��¼���� */
static void print_record_detail(const Record* rec) {
    printf("\n=== ��ǰ��¼���� ===\n");
    printf("ѧ��:       %s\n", rec->student_id);
    printf("����:       %s\n", rec->name);
    printf("ѧԺ:       %s\n", rec->college);
    printf("�γ̱��:   %s\n", rec->course_id);
    printf("�γ�����:   %s\n", rec->course_name);
    printf("ѧ��:       %.1f\n", rec->credit);
    printf("ѡ��ѧ��:   %s\n", rec->semester);
    printf("ѡ������:   %s\n", rec->select_date);
    printf("�ɼ�:       %d\n", rec->score);
    printf("====================\n");
}

/* ����ʽ�޸ļ�¼�ֶ� */
bool modify_record_interactive(Record* rec) {
    int choice;
    bool modified = false;
    char input_buf[MAX_STR_LEN];
    int new_score;

    if (!rec) return false;

    print_record_detail(rec);

    while (1) {
        printf("\n--- �޸��ֶ� ---\n");
        printf("1. �ɼ�\n");
        printf("2. ѡ��ѧ��\n");
        printf("3. ѡ������\n");
        printf("4. ����\n");
        printf("5. ѧԺ\n");
        printf("6. �γ�����\n");
        printf("0. ����޸�\n");
        printf("��ѡ��Ҫ�޸ĵ��ֶ�: ");
        scanf("%d", &choice);

        switch (choice) {
            case MODIFY_SCORE:
                printf("��ǰ�ɼ�: %d\n", rec->score);
                printf("�������³ɼ�(0-100): ");
                scanf("%d", &new_score);
                if (new_score < 0 || new_score > 100) {
                    printf("�ɼ�������Χ���޸�ʧ��\n");
                } else {
                    rec->score = new_score;
                    printf("�ɼ��Ѹ���Ϊ %d\n", rec->score);
                    modified = true;
                }
                break;

            case MODIFY_SEMESTER:
                printf("��ǰѧ��: %s\n", rec->semester);
                printf("��������ѧ��(��ʽ YYYY-XX���� 2025-01): ");
                scanf("%s", input_buf);
                if (strlen(input_buf) != 7 || input_buf[4] != '-') {
                    printf("ѧ�ڸ�ʽ�����޸�ʧ��\n");
                } else {
                    strncpy(rec->semester, input_buf, SEMESTER_LEN - 1);
                    rec->semester[SEMESTER_LEN - 1] = '\0';
                    printf("ѧ���Ѹ���Ϊ %s\n", rec->semester);
                    modified = true;
                }
                break;

            case MODIFY_SELECT_DATE:
                printf("��ǰѡ������: %s\n", rec->select_date);
                printf("������������(��ʽ YYYY-MM-DD): ");
                scanf("%s", input_buf);
                if (!is_valid_date(input_buf)) {
                    printf("���ڸ�ʽ�����޸�ʧ��\n");
                } else {
                    strncpy(rec->select_date, input_buf, DATE_LEN - 1);
                    rec->select_date[DATE_LEN - 1] = '\0';
                    printf("ѡ�������Ѹ���Ϊ %s\n", rec->select_date);
                    modified = true;
                }
                break;

            case MODIFY_NAME:
                printf("��ǰ����: %s\n", rec->name);
                printf("������������: ");
                scanf("%s", input_buf);
                if (strlen(input_buf) == 0) {
                    printf("��������Ϊ�գ��޸�ʧ��\n");
                } else {
                    strncpy(rec->name, input_buf, MAX_STR_LEN - 1);
                    rec->name[MAX_STR_LEN - 1] = '\0';
                    printf("�����Ѹ���Ϊ %s\n", rec->name);
                    modified = true;
                }
                break;

            case MODIFY_COLLEGE:
                printf("��ǰѧԺ: %s\n", rec->college);
                printf("��������ѧԺ: ");
                scanf("%s", input_buf);
                if (strlen(input_buf) == 0) {
                    printf("ѧԺ����Ϊ�գ��޸�ʧ��\n");
                } else {
                    strncpy(rec->college, input_buf, MAX_STR_LEN - 1);
                    rec->college[MAX_STR_LEN - 1] = '\0';
                    printf("ѧԺ�Ѹ���Ϊ %s\n", rec->college);
                    modified = true;
                }
                break;

            case MODIFY_COURSE_NAME:
                printf("��ǰ�γ�����: %s\n", rec->course_name);
                printf("�������¿γ�����: ");
                scanf("%s", input_buf);
                if (strlen(input_buf) == 0) {
                    printf("�γ����Ʋ���Ϊ�գ��޸�ʧ��\n");
                } else {
                    strncpy(rec->course_name, input_buf, MAX_STR_LEN - 1);
                    rec->course_name[MAX_STR_LEN - 1] = '\0';
                    printf("�γ������Ѹ���Ϊ %s\n", rec->course_name);
                    modified = true;
                }
                break;

            case 0:
                if (modified) {
                    printf("\n--- �޸ĺ��¼���� ---\n");
                    print_record_detail(rec);
                }
                return modified;

            default:
                printf("��Чѡ��\n");
        }
    }
}
