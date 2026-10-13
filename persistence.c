#include "persistence.h"

#ifdef _WIN32
  #include <direct.h>
#endif

/* ========== �ڲ�������ȥ�� ="..." ���� ========== */

static void strip_csv_wrapper(char* str) {
    int len;
    if (str[0] == '=' && str[1] == '"') {
        len = strlen(str);
        if (len >= 4 && str[len - 1] == '"') {
            memmove(str, str + 2, len - 3);
            str[len - 3] = '\0';
        }
    }
}

/* ========== ����CSV ========== */

int load_from_csv(LinkedList* list, const char* filename) {
    FILE* fp;
    char line[1024];
    int count;
    char* token;

    fp = fopen(filename, "r");
    if (!fp) return ERROR;

    fgets(line, sizeof(line), fp);

    count = 0;
    while (fgets(line, sizeof(line), fp)) {
        Record rec;
        char clean_buf[128];

        token = strtok(line, ",");
        if (!token) continue;
        strcpy(rec.student_id, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(rec.name, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(rec.college, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(rec.course_id, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(rec.course_name, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        rec.credit = (float)atof(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(clean_buf, token);
        strip_csv_wrapper(clean_buf);
        strncpy(rec.semester, clean_buf, SEMESTER_LEN - 1);
        rec.semester[SEMESTER_LEN - 1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(clean_buf, token);
        strip_csv_wrapper(clean_buf);
        strncpy(rec.select_date, clean_buf, DATE_LEN - 1);
        rec.select_date[DATE_LEN - 1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue;
        rec.score = atoi(token);

        list_insert(list, &rec);
        count++;
    }

    fclose(fp);
    return count;
}

/* ========== ����CSV ========== */

int save_to_csv(LinkedList* list, const char* filename) {
    FILE* fp;
    ListNode* cur;

    fp = fopen(filename, "w");
    if (!fp) return ERROR;

    fprintf(fp, "ѧ��,����,ѧԺ,�γ̱��,�γ�����,ѧ��,ѡ��ѧ��,ѡ������,�ɼ�\n");

    cur = list->head;
    while (cur) {
        fprintf(fp, "%s,%s,%s,%s,%s,%.1f,=\"%s\",=\"%s\",%d\n",
                cur->data.student_id,
                cur->data.name,
                cur->data.college,
                cur->data.course_id,
                cur->data.course_name,
                cur->data.credit,
                cur->data.semester,
                cur->data.select_date,
                cur->data.score);
        cur = cur->next;
    }

    fclose(fp);
    return OK;
}

/* ========== Ŀ¼���� ========== */

void ensure_directory(const char* dir) {
    struct stat st;
    if (stat(dir, &st) == -1) {
#ifdef _WIN32
        _mkdir(dir);
#else
        mkdir(dir, 0755);
#endif
    }
}

/* ========== ɸѡ�ļ������� ========== */

void generate_filter_filename(const char* dir,
                              const char* criteria_desc,
                              char* buf, int buf_size) {
    time_t now;
    struct tm* t;
    char ts[32];

    now = time(NULL);
    t = localtime(&now);
    sprintf(ts, "%04d%02d%02d_%02d%02d%02d",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec);

    if (criteria_desc && strlen(criteria_desc) > 0) {
        snprintf(buf, buf_size, "%s/filter_%s_%s.csv", dir, criteria_desc, ts);
    } else {
        snprintf(buf, buf_size, "%s/filter_%s.csv", dir, ts);
    }
}
