#include "performance.h"
#include "data_generator.h"
#include "linked_list.h"
#include "avl_tree.h"
#include "hash_table.h"

/* ��ȡ��ǰ���뼶ʱ�� */
static long get_time_ms() {
    return (long)clock() * 1000 / CLOCKS_PER_SEC;
}

void run_performance_test(int data_size) {
    Record* records;
    int actual_count;
    LinkedList* list;
    AvlTree* avl;
    HashTable* hash;

    long t_start, t_end;
    long list_insert_ms, avl_insert_ms, hash_insert_ms;
    long list_find_ms, avl_find_ms, hash_find_ms;
    long list_delete_ms, avl_delete_ms, hash_delete_ms;
    long list_update_ms, avl_update_ms, hash_update_ms;

    int i;
    int sample_start, sample_end;
    int update_count;

    printf("\n===== ���ܶԱȲ��ԣ����ݹ�ģ��%d����=====\n", data_size);

    /* ���ɲ������� */
    records = generate_records(data_size, &actual_count);
    if (!records || actual_count <= 0) {
        printf("��������ʧ��\n");
        return;
    }

    /* ���������ݽṹ */
    list = list_create();
    avl = avl_create();
    hash = hash_create();

    /* ������� */
    printf("\n--- �������� ---\n");

    t_start = get_time_ms();
    for (i = 0; i < actual_count; i++) {
        list_insert(list, &records[i]);
    }
    t_end = get_time_ms();
    list_insert_ms = t_end - t_start;
    printf("����: %ld ms\n", list_insert_ms);

    t_start = get_time_ms();
    for (i = 0; i < actual_count; i++) {
        avl_insert(avl, &records[i]);
    }
    t_end = get_time_ms();
    avl_insert_ms = t_end - t_start;
    printf("AVL��: %ld ms\n", avl_insert_ms);

    t_start = get_time_ms();
    for (i = 0; i < actual_count; i++) {
        hash_insert(hash, &records[i]);
    }
    t_end = get_time_ms();
    hash_insert_ms = t_end - t_start;
    printf("��ϣ��: %ld ms\n", hash_insert_ms);

    /* ���Ҳ��ԣ�ȡ�м�100����Ϊ��ѯ������ */
    printf("\n--- �������ܣ�100�β�ѯ�� ---\n");
    sample_start = actual_count / 2;
    sample_end = sample_start + 100;
    if (sample_end > actual_count) sample_end = actual_count;

    t_start = get_time_ms();
    for (i = sample_start; i < sample_end; i++) {
        list_find(list, records[i].student_id, records[i].course_id);
    }
    t_end = get_time_ms();
    list_find_ms = t_end - t_start;
    printf("����: %ld ms\n", list_find_ms);

    t_start = get_time_ms();
    for (i = sample_start; i < sample_end; i++) {
        avl_find(avl, records[i].student_id, records[i].course_id);
    }
    t_end = get_time_ms();
    avl_find_ms = t_end - t_start;
    printf("AVL��: %ld ms\n", avl_find_ms);

    t_start = get_time_ms();
    for (i = sample_start; i < sample_end; i++) {
        hash_find(hash, records[i].student_id, records[i].course_id);
    }
    t_end = get_time_ms();
    hash_find_ms = t_end - t_start;
    printf("��ϣ��: %ld ms\n", hash_find_ms);

    /* ɾ������ */
    printf("\n--- ɾ�����ܣ�100��ɾ���� ---\n");
    t_start = get_time_ms();
    for (i = sample_start; i < sample_end; i++) {
        list_delete(list, records[i].student_id, records[i].course_id);
    }
    t_end = get_time_ms();
    list_delete_ms = t_end - t_start;
    printf("����: %ld ms\n", list_delete_ms);

    t_start = get_time_ms();
    for (i = sample_start; i < sample_end; i++) {
        avl_delete(avl, records[i].student_id, records[i].course_id);
    }
    t_end = get_time_ms();
    avl_delete_ms = t_end - t_start;
    printf("AVL��: %ld ms\n", avl_delete_ms);

    t_start = get_time_ms();
    for (i = sample_start; i < sample_end; i++) {
        hash_delete(hash, records[i].student_id, records[i].course_id);
    }
    t_end = get_time_ms();
    hash_delete_ms = t_end - t_start;
    printf("��ϣ��: %ld ms\n", hash_delete_ms);

    /* �޸Ĳ��ԣ�100���޸ģ� */
    printf("\n--- �޸����ܣ�100���޸ģ� ---\n");

    update_count = 0;
    t_start = get_time_ms();
    for (i = sample_start; i < sample_end; i++) {
        if (list_update(list, records[i].student_id, records[i].course_id, 80) == OK)
            update_count++;
    }
    t_end = get_time_ms();
    list_update_ms = t_end - t_start;
    printf("����: %ld ms (�ɹ� %d ��)\n", list_update_ms, update_count);

    update_count = 0;
    t_start = get_time_ms();
    for (i = sample_start; i < sample_end; i++) {
        if (avl_update(avl, records[i].student_id, records[i].course_id, 80) == OK)
            update_count++;
    }
    t_end = get_time_ms();
    avl_update_ms = t_end - t_start;
    printf("AVL��: %ld ms (�ɹ� %d ��)\n", avl_update_ms, update_count);

    update_count = 0;
    t_start = get_time_ms();
    for (i = sample_start; i < sample_end; i++) {
        if (hash_update(hash, records[i].student_id, records[i].course_id, 80) == OK)
            update_count++;
    }
    t_end = get_time_ms();
    hash_update_ms = t_end - t_start;
    printf("��ϣ��: %ld ms (�ɹ� %d ��)\n", hash_update_ms, update_count);

    /* ���ܱ��� */
    printf("\n\n=== ���ܻ��ܱ�����λ��ms�� ===\n");
    printf("%-12s %-10s %-10s %-10s\n", "����", "����", "AVL��", "��ϣ��");
    printf("--------------------------------------------\n");
    printf("%-12s %-10ld %-10ld %-10ld\n", "����(ȫ��)", list_insert_ms, avl_insert_ms, hash_insert_ms);
    printf("%-12s %-10ld %-10ld %-10ld\n", "����(100��)", list_find_ms, avl_find_ms, hash_find_ms);
    printf("%-12s %-10ld %-10ld %-10ld\n", "ɾ��(100��)", list_delete_ms, avl_delete_ms, hash_delete_ms);
    printf("%-12s %-10ld %-10ld %-10ld\n", "�޸�(100��)", list_update_ms, avl_update_ms, hash_update_ms);

    /* �ͷ���Դ */
    list_destroy(list);
    avl_destroy(avl);
    hash_destroy(hash);
    free_records(records);
}
