#include <stdio.h>
#include <time.h>
#include "common.h"
#include "data_generator.h"
#include "linked_list.h"
#include "data_store.h"
#include "persistence.h"
#include "query.h"
#include "performance.h"

#define DATA_FILE  "records.csv"
#define FILTER_DIR "filtered_data"

static DataStore* store = NULL;

void print_menu(void) {
    printf("\n===== У԰ѡ�μ�¼����ϵͳ =====\n");
    printf("��ǰ���ݽṹ: %s\n", ds_type_name(store));
    printf("1.  ���ɲ�������\n");
    printf("2.  ���ļ���������\n");
    printf("3.  �����¼\n");
    printf("4.  ɾ����¼\n");
    printf("5.  ���Ҽ�¼\n");
    printf("6.  ������ɸѡ\n");
    printf("7.  ͳ�Ʒ���\n");
    printf("8.  ���ܶԱȲ���\n");
    printf("9.  �޸ļ�¼\n");
    printf("10. �г���ǰ��¼\n");
    printf("11. �л����ݽṹ\n");
    printf("0.  ���沢�˳�\n");
    printf("������ѡ��: ");
}

/* ====================================================================
 *  ɾ���Ӳ˵�
 * ==================================================================== */

static void delete_submenu(void) {
    int choice;
    char op[8];

    while (1) {
        printf("\n----- ɾ����¼ -----\n");
        printf("��ǰ�ṹ: %s����¼��: %d\n", ds_type_name(store), ds_size(store));
        printf("1. ɾ��������¼����ѧ��+�γ̺ţ�\n");
        printf("2. ��ѧԺ����ɾ��\n");
        printf("3. ��ѧ������ɾ��\n");
        printf("4. ���γ̱������ɾ��\n");
        printf("5. ��ѧ������ɾ����ɾ��ĳѧ��ȫ��ѡ�Σ�\n");
        printf("6. ���ɼ���Χ����ɾ��\n");
        printf("7. ����ɾ�����ڼ�¼\n");
        printf("0. �������˵�\n");
        printf("��ѡ��: ");
        scanf("%d", &choice);

        if (choice == 0) break;

        switch (choice) {

            case 1: {
                char sid[ID_LEN], cid[COURSE_ID_LEN];
                printf("������ѧ�� �γ̱��: ");
                scanf("%s %s", sid, cid);
                if (ds_delete(store, sid, cid) == OK)
                    printf("ɾ���ɹ�\n");
                else
                    printf("δ�ҵ���¼\n");
                break;
            }

            case 2: {
                char college[MAX_STR_LEN];
                int cnt;
                printf("������ѧԺ����: ");
                scanf("%s", college);
                cnt = ds_count_by_college(store, college);
                if (cnt == 0) { printf("δ�ҵ�ƥ���¼\n"); break; }
                printf("���ҵ� %d ������ [%s] �ļ�¼��ȷ��ɾ����(Y/N): ",
                       cnt, college);
                scanf("%7s", op);
                if (op[0] == 'Y' || op[0] == 'y')
                    printf("��ɾ�� %d ����¼\n",
                           ds_delete_by_college(store, college));
                break;
            }

            case 3: {
                char semester[SEMESTER_LEN];
                int cnt;
                printf("������ѧ��(�� 2024-01): ");
                scanf("%s", semester);
                cnt = ds_count_by_semester(store, semester);
                if (cnt == 0) { printf("δ�ҵ�ƥ���¼\n"); break; }
                printf("���ҵ� %d �� [%s] ѧ�ڵļ�¼��ȷ��ɾ����(Y/N): ",
                       cnt, semester);
                scanf("%7s", op);
                if (op[0] == 'Y' || op[0] == 'y')
                    printf("��ɾ�� %d ����¼\n",
                           ds_delete_by_semester(store, semester));
                break;
            }

            case 4: {
                char cid[COURSE_ID_LEN];
                int cnt;
                printf("������γ̱��: ");
                scanf("%s", cid);
                cnt = ds_count_by_course(store, cid);
                if (cnt == 0) { printf("δ�ҵ�ƥ���¼\n"); break; }
                printf("���ҵ� %d ���γ� [%s] �ļ�¼��ȷ��ɾ����(Y/N): ",
                       cnt, cid);
                scanf("%7s", op);
                if (op[0] == 'Y' || op[0] == 'y')
                    printf("��ɾ�� %d ����¼\n",
                           ds_delete_by_course(store, cid));
                break;
            }

            case 5: {
                char sid[ID_LEN];
                int cnt;
                printf("������ѧ��: ");
                scanf("%s", sid);
                cnt = ds_count_by_student(store, sid);
                if (cnt == 0) { printf("δ�ҵ�ƥ���¼\n"); break; }
                printf("���ҵ�ѧ�� [%s] �� %d ��ѡ�μ�¼��ȷ��ȫ��ɾ����(Y/N): ",
                       sid, cnt);
                scanf("%7s", op);
                if (op[0] == 'Y' || op[0] == 'y')
                    printf("��ɾ�� %d ����¼\n",
                           ds_delete_by_student(store, sid));
                break;
            }

            case 6: {
                int min_s, max_s, cnt;
                printf("��������ͷ�: ");
                scanf("%d", &min_s);
                printf("��������߷�: ");
                scanf("%d", &max_s);
                if (min_s > max_s) {
                    int tmp = min_s; min_s = max_s; max_s = tmp;
                }
                cnt = ds_count_by_score(store, min_s, max_s);
                if (cnt == 0) { printf("δ�ҵ�ƥ���¼\n"); break; }
                printf("���ҵ� %d ���ɼ��� [%d-%d] �ļ�¼��ȷ��ɾ����(Y/N): ",
                       cnt, min_s, max_s);
                scanf("%7s", op);
                if (op[0] == 'Y' || op[0] == 'y')
                    printf("��ɾ�� %d ����¼\n",
                           ds_delete_by_score(store, min_s, max_s));
                break;
            }

            case 7: {
                char base_date[DATE_LEN];
                char input_buf[DATE_LEN + 2];
                int len, cnt, years;
                printf("�������׼����(YYYY-MM-DD��ֱ�ӻس�ʹ��Ĭ��2026-09-01): ");
                getchar();
                fgets(input_buf, sizeof(input_buf), stdin);
                len = strlen(input_buf);
                if (len > 0 && input_buf[len - 1] == '\n')
                    input_buf[len - 1] = '\0';
                if (strlen(input_buf) == 0) {
                    strcpy(base_date, "2026-09-01");
                } else {
                    if (!is_valid_date(input_buf)) {
                        printf("���ڸ�ʽ����\n"); break;
                    }
                    strcpy(base_date, input_buf);
                }
                printf("�������������(ֱ�ӻس�Ĭ��3��): ");
                fgets(input_buf, sizeof(input_buf), stdin);
                len = strlen(input_buf);
                if (len > 0 && input_buf[len - 1] == '\n')
                    input_buf[len - 1] = '\0';
                years = (strlen(input_buf) == 0) ? 3 : atoi(input_buf);
                if (years <= 0) years = 3;

                cnt = ds_count_expired(store, base_date, years);
                printf("��׼����: %s��ɾ�� %d ��ǰ�ļ�¼���� %d ��",
                       base_date, years, cnt);
                if (cnt == 0) { printf("\n����ɾ��\n"); break; }
                printf("��ȷ��ɾ����(Y/N): ");
                scanf("%7s", op);
                if (op[0] == 'Y' || op[0] == 'y')
                    printf("��ɾ�� %d �����ڼ�¼\n",
                           ds_delete_expired(store, base_date, years));
                break;
            }

            default:
                printf("��Чѡ��\n");
        }
    }
}

/* ====================================================================
 *  ������
 * ==================================================================== */

int main(void) {
    int choice;

    store = ds_create(STORE_LINKED_LIST);
    if (!store) { printf("��ʼ��ʧ��\n"); return 1; }

    while (1) {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {

            case 1: {
                int n, count, i;
                Record* recs;
                printf("��������������: ");
                scanf("%d", &n);
                count = 0;
                recs = generate_records(n, &count);
                for (i = 0; i < count; i++) ds_insert(store, &recs[i]);
                free_records(recs);
                printf("�ɹ����� %d ����¼����ǰ�ṹ: %s��\n",
                       ds_size(store), ds_type_name(store));
                break;
            }

            case 2: {
                LinkedList* temp;
                ListNode* cur;
                int n;
                temp = list_create();
                n = load_from_csv(temp, DATA_FILE);
                if (n <= 0) {
                    printf("����ʧ�ܻ��ļ�Ϊ��\n");
                    list_destroy(temp); break;
                }
                cur = temp->head;
                while (cur) { ds_insert(store, &cur->data); cur = cur->next; }
                list_destroy(temp);
                printf("������ɣ��� %d ����¼����ǰ�ṹ: %s��\n",
                       ds_size(store), ds_type_name(store));
                break;
            }

            case 3: {
                Record rec;
                printf("������ѧ�� �γ̱�� �ɼ�: ");
                scanf("%s %s %d", rec.student_id, rec.course_id, &rec.score);
                strcpy(rec.name, "����ѧ��");
                strcpy(rec.college, "�����ѧԺ");
                strcpy(rec.course_name, "���ݽṹ");
                rec.credit = 3.0f;
                strcpy(rec.semester, "2025-02");
                strcpy(rec.select_date, "2025-09-01");
                if (ds_insert(store, &rec) == OK) printf("����ɹ�\n");
                else printf("����ʧ��\n");
                break;
            }

            case 4:
                delete_submenu();
                break;

            case 5: {
                char sid[ID_LEN], cid[COURSE_ID_LEN];
                Record* rec;
                printf("������ѧ�� �γ̱��: ");
                scanf("%s %s", sid, cid);
                rec = ds_find(store, sid, cid);
                if (rec) { printf("\n�ҵ���¼:\n"); print_record(rec); }
                else printf("δ�ҵ���¼\n");
                break;
            }

            case 6: {
                FilterParams params;
                SortKey keys[3];
                int key_count = 0;
                LinkedList* source;
                LinkedList* result;
                Record* arr = NULL;
                ListNode* cur;
                int sub_choice, i;
                int is_sorted = 0;
                char desc_buf[256];
                int desc_len = 0;

                memset(&params, 0, sizeof(FilterParams));
                memset(desc_buf, 0, sizeof(desc_buf));

                printf("\n--- ������ɸѡ ---\n");

                printf("�Ƿ񰴿γ�����ɸѡ��(1-�� / 0-��): ");
                scanf("%d", &sub_choice);
                if (sub_choice) {
                    params.use_course_name = 1;
                    printf("�Ƿ�ģ��ƥ�䣿(1-�� / 0-��): ");
                    scanf("%d", &sub_choice);
                    params.fuzzy_course = sub_choice ? 1 : 0;
                    printf("������γ�����: ");
                    scanf("%s", params.course_name);
                    desc_len += sprintf(desc_buf + desc_len,
                                        "C%s_", params.course_name);
                }

                printf("�Ƿ�ѧ��ɸѡ��(1-�� / 0-��): ");
                scanf("%d", &sub_choice);
                if (sub_choice) {
                    params.use_semester = 1;
                    printf("������ѧ��(�� 2024-01): ");
                    scanf("%s", params.semester);
                    desc_len += sprintf(desc_buf + desc_len,
                                        "S%s_", params.semester);
                }

                printf("�Ƿ񰴳ɼ���Χɸѡ��(1-�� / 0-��): ");
                scanf("%d", &sub_choice);
                if (sub_choice) {
                    params.use_score_range = 1;
                    printf("��������ͷ�: ");
                    scanf("%d", &params.score_min);
                    printf("��������߷�: ");
                    scanf("%d", &params.score_max);
                    desc_len += sprintf(desc_buf + desc_len,
                                        "R%d-%d_",
                                        params.score_min, params.score_max);
                }

                printf("�Ƿ�ѧԺɸѡ��(1-�� / 0-��): ");
                scanf("%d", &sub_choice);
                if (sub_choice) {
                    params.use_college = 1;
                    printf("������ѧԺ����: ");
                    scanf("%s", params.college);
                    desc_len += sprintf(desc_buf + desc_len,
                                        "G%s_", params.college);
                }

                source = ds_export_to_list(store);
                result = filter_records(source, &params);
                list_destroy(source);

                if (!result) { printf("ɸѡʧ��\n"); break; }
                printf("\nɸѡ����� %d ����¼\n", list_size(result));
                if (list_size(result) == 0) {
                    list_destroy(result); break;
                }

                printf("�Ƿ�����(1-�� / 0-��): ");
                scanf("%d", &sub_choice);
                if (sub_choice && list_size(result) > 1) {
                    printf("��ѡ�����ֶ�: score / student_id / course_id\n");
                    printf("����������ؼ�������(1-3): ");
                    scanf("%d", &key_count);
                    if (key_count > 3) key_count = 3;
                    for (i = 0; i < key_count; i++) {
                        printf("��%d�������ֶ���: ", i + 1);
                        scanf("%s", keys[i].field);
                        printf("����(1-�� / 0-��): ");
                        scanf("%d", &sub_choice);
                        keys[i].descending = sub_choice ? 1 : 0;
                    }
                    arr = (Record*)malloc(list_size(result) * sizeof(Record));
                    if (arr) {
                        i = 0;
                        cur = result->head;
                        while (cur) { arr[i++] = cur->data; cur = cur->next; }
                        sort_records(arr, list_size(result), keys, key_count);
                        is_sorted = 1;
                        printf("\n%-12s %-8s %-20s %-8s %-20s %4s %-7s %-10s %4s\n",
                               "ѧ��","����","ѧԺ","�γ̺�","�γ���",
                               "ѧ��","ѧ��","ѡ������","�ɼ�");
                        for (i = 0; i < list_size(result); i++)
                            print_record(&arr[i]);
                    }
                } else {
                    printf("\n%-12s %-8s %-20s %-8s %-20s %4s %-7s %-10s %4s\n",
                           "ѧ��","����","ѧԺ","�γ̺�","�γ���",
                           "ѧ��","ѧ��","ѡ������","�ɼ�");
                    list_traverse(result, print_record);
                }

                printf("\n�Ƿ񱣴�ɸѡ������ļ���(1-�� / 0-��): ");
                scanf("%d", &sub_choice);
                if (sub_choice) {
                    char save_path[512];
                    char filename[256];
                    ensure_directory(FILTER_DIR);
                    printf("���淽ʽ: 1.�Զ������ļ���  2.�ֶ������ļ���\n��ѡ��: ");
                    scanf("%d", &sub_choice);
                    if (sub_choice == 2) {
                        printf("�������ļ���(��·��): ");
                        scanf("%s", save_path);
                    } else {
                        if (desc_len > 0 && desc_buf[desc_len - 1] == '_')
                            desc_buf[desc_len - 1] = '\0';
                        generate_filter_filename(FILTER_DIR, desc_buf,
                                                 filename, sizeof(filename));
                        strcpy(save_path, filename);
                    }
                    if (is_sorted && arr) {
                        LinkedList* sorted_list = list_create();
                        if (sorted_list) {
                            for (i = 0; i < list_size(result); i++)
                                list_insert(sorted_list, &arr[i]);
                            save_to_csv(sorted_list, save_path);
                            list_destroy(sorted_list);
                        }
                    } else {
                        save_to_csv(result, save_path);
                    }
                    printf("ɸѡ����ѱ�����: %s\n", save_path);
                }

                if (is_sorted && arr) free(arr);
                list_destroy(result);
                break;
            }

            case 7: {
                int sub;
                LinkedList* temp;
                printf("\n1. �γ�ѡ������ͳ��\n2. �ɼ��ֲ�ͳ��\n��ѡ��: ");
                scanf("%d", &sub);
                temp = ds_export_to_list(store);
                if (sub == 1) stat_course_enrollment(temp);
                else if (sub == 2) stat_score_distribution(temp);
                list_destroy(temp);
                break;
            }

            case 8: {
                int size;
                printf("������������ݹ�ģ(100/1000/10000/100000): ");
                scanf("%d", &size);
                run_performance_test(size);
                break;
            }

            case 9: {
                char sid[ID_LEN], cid[COURSE_ID_LEN];
                Record* rec;
                printf("\n--- �޸ļ�¼ ---\n");
                printf("������ѧ��: ");
                scanf("%s", sid);
                printf("������γ̱��: ");
                scanf("%s", cid);
                rec = ds_find(store, sid, cid);
                if (!rec) { printf("δ�ҵ��ü�¼\n"); break; }
                if (modify_record_interactive(rec))
                    printf("\n��¼�޸ĳɹ�\n");
                else
                    printf("\nδ���κ��޸�\n");
                break;
            }

            case 10: {
                int total, page_size, total_pages, current_page;
                int start_idx, end_idx, i;
                char input[16];
                Record* arr;
                int arr_count;

                total = ds_size(store);
                if (total == 0) { printf("\n��ǰ���κμ�¼\n"); break; }

                printf("\n��ǰ�� %d ����¼�����ݽṹ: %s��\n",
                       total, ds_type_name(store));
                printf("������ÿҳ��ʾ����(ֱ�ӻس�Ĭ��20): ");
                getchar();
                fgets(input, sizeof(input), stdin);
                if (input[0] == '\n' || input[0] == '\0') page_size = 20;
                else { page_size = atoi(input); if (page_size <= 0) page_size = 20; }

                total_pages = (total + page_size - 1) / page_size;
                current_page = 1;

                arr = ds_export_array(store, &arr_count);
                if (!arr || arr_count <= 0) {
                    printf("����ʧ��\n");
                    if (arr) free(arr);
                    break;
                }

                while (1) {
                    start_idx = (current_page - 1) * page_size;
                    end_idx = start_idx + page_size;
                    if (end_idx > arr_count) end_idx = arr_count;

                    printf("\n===== �� %d / %d ҳ���� %d - %d ������ %d ����=====\n",
                           current_page, total_pages,
                           start_idx + 1, end_idx, arr_count);
                    printf("%-12s %-8s %-20s %-8s %-20s %4s %-7s %-10s %4s\n",
                           "ѧ��","����","ѧԺ","�γ̺�","�γ���",
                           "ѧ��","ѧ��","ѡ������","�ɼ�");
                    printf("------------------------------------------------------------"
                           "-----------------------------------------------\n");
                    for (i = start_idx; i < end_idx; i++)
                        print_record(&arr[i]);

                    printf("\n--- ");
                    if (current_page > 1) printf("��һҳ:P  ");
                    printf("��ת:G  ");
                    if (current_page < total_pages) printf("��һҳ:N  ");
                    printf("�˳�:Q ---\n");
                    printf("���������: ");
                    scanf("%s", input);

                    if (input[0] == 'Q' || input[0] == 'q') break;
                    else if ((input[0] == 'N' || input[0] == 'n')
                             && current_page < total_pages) current_page++;
                    else if ((input[0] == 'P' || input[0] == 'p')
                             && current_page > 1) current_page--;
                    else if (input[0] == 'G' || input[0] == 'g') {
                        int target;
                        printf("������Ŀ��ҳ��(1-%d): ", total_pages);
                        scanf("%d", &target);
                        if (target >= 1 && target <= total_pages)
                            current_page = target;
                        else printf("ҳ�볬����Χ\n");
                    } else printf("��Ч����\n");
                }
                free(arr);
                break;
            }

            case 11: {
                int new_type;
                printf("\n--- �л����ݽṹ ---\n");
                printf("��ǰ�ṹ: %s\n", ds_type_name(store));
                printf("1. ������˫��������\n");
                printf("2. AVL������ƽ�������������\n");
                printf("3. ��ϣ������������\n");
                printf("4. ��̬���飨��������+���ֲ��ң�\n");
                printf("0. ȡ��\n");
                printf("��ѡ��: ");
                scanf("%d", &new_type);

                if (new_type == 0) { printf("��ȡ��\n"); break; }
                if (new_type < 1 || new_type > 4) { printf("��Чѡ��\n"); break; }
                if ((enum StoreType)new_type == store->type) {
                    printf("��ǰ���� %s�������л�\n", ds_type_name(store));
                    break;
                }

                printf("���ڽ� %d ����¼�� %s Ǩ�Ƶ� ",
                       ds_size(store), ds_type_name(store));
                switch (new_type) {
                    case 1: printf("����...\n"); break;
                    case 2: printf("AVL��...\n"); break;
                    case 3: printf("��ϣ��...\n"); break;
                    case 4: printf("��̬����...\n"); break;
                }

                if (ds_switch_type(store, (enum StoreType)new_type) == OK)
                    printf("�л��ɹ�����ǰ�ṹ: %s����¼��: %d\n",
                           ds_type_name(store), ds_size(store));
                else
                    printf("�л�ʧ��\n");
                break;
            }

            case 0: {
                LinkedList* temp;
                temp = ds_export_to_list(store);
                save_to_csv(temp, DATA_FILE);
                list_destroy(temp);
                ds_destroy(store);
                printf("�����ѱ��棬�����˳�\n");
                return 0;
            }

            default:
                printf("��Чѡ��\n");
        }
    }
}
