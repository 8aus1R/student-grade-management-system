#include "performance.h"
#include "data_generator.h"
#include "linked_list.h"
#include "avl_tree.h"
#include "hash_table.h"

/* 获取当前毫秒级时间 */
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

    printf("\n===== 性能对比测试（数据规模：%d条）=====\n", data_size);

    /* 生成测试数据 */
    records = generate_records(data_size, &actual_count);
    if (!records || actual_count <= 0) {
        printf("数据生成失败\n");
        return;
    }

    /* 创建各数据结构 */
    list = list_create();
    avl = avl_create();
    hash = hash_create();

    /* 插入测试 */
    printf("\n--- 插入性能 ---\n");

    t_start = get_time_ms();
    for (i = 0; i < actual_count; i++) {
        list_insert(list, &records[i]);
    }
    t_end = get_time_ms();
    list_insert_ms = t_end - t_start;
    printf("链表: %ld ms\n", list_insert_ms);

    t_start = get_time_ms();
    for (i = 0; i < actual_count; i++) {
        avl_insert(avl, &records[i]);
    }
    t_end = get_time_ms();
    avl_insert_ms = t_end - t_start;
    printf("AVL树: %ld ms\n", avl_insert_ms);

    t_start = get_time_ms();
    for (i = 0; i < actual_count; i++) {
        hash_insert(hash, &records[i]);
    }
    t_end = get_time_ms();
    hash_insert_ms = t_end - t_start;
    printf("哈希表: %ld ms\n", hash_insert_ms);

    /* 查找测试（取中间100条作为查询样本） */
    printf("\n--- 查找性能（100次查询） ---\n");
    sample_start = actual_count / 2;
    sample_end = sample_start + 100;
    if (sample_end > actual_count) sample_end = actual_count;

    t_start = get_time_ms();
    for (i = sample_start; i < sample_end; i++) {
        list_find(list, records[i].student_id, records[i].course_id);
    }
    t_end = get_time_ms();
    list_find_ms = t_end - t_start;
    printf("链表: %ld ms\n", list_find_ms);

    t_start = get_time_ms();
    for (i = sample_start; i < sample_end; i++) {
        avl_find(avl, records[i].student_id, records[i].course_id);
    }
    t_end = get_time_ms();
    avl_find_ms = t_end - t_start;
    printf("AVL树: %ld ms\n", avl_find_ms);

    t_start = get_time_ms();
    for (i = sample_start; i < sample_end; i++) {
        hash_find(hash, records[i].student_id, records[i].course_id);
    }
    t_end = get_time_ms();
    hash_find_ms = t_end - t_start;
    printf("哈希表: %ld ms\n", hash_find_ms);

    /* 删除测试 */
    printf("\n--- 删除性能（100次删除） ---\n");
    t_start = get_time_ms();
    for (i = sample_start; i < sample_end; i++) {
        list_delete(list, records[i].student_id, records[i].course_id);
    }
    t_end = get_time_ms();
    list_delete_ms = t_end - t_start;
    printf("链表: %ld ms\n", list_delete_ms);

    t_start = get_time_ms();
    for (i = sample_start; i < sample_end; i++) {
        avl_delete(avl, records[i].student_id, records[i].course_id);
    }
    t_end = get_time_ms();
    avl_delete_ms = t_end - t_start;
    printf("AVL树: %ld ms\n", avl_delete_ms);

    t_start = get_time_ms();
    for (i = sample_start; i < sample_end; i++) {
        hash_delete(hash, records[i].student_id, records[i].course_id);
    }
    t_end = get_time_ms();
    hash_delete_ms = t_end - t_start;
    printf("哈希表: %ld ms\n", hash_delete_ms);

    /* 修改测试（100次修改） */
    printf("\n--- 修改性能（100次修改） ---\n");

    update_count = 0;
    t_start = get_time_ms();
    for (i = sample_start; i < sample_end; i++) {
        if (list_update(list, records[i].student_id, records[i].course_id, 80) == OK)
            update_count++;
    }
    t_end = get_time_ms();
    list_update_ms = t_end - t_start;
    printf("链表: %ld ms (成功 %d 次)\n", list_update_ms, update_count);

    update_count = 0;
    t_start = get_time_ms();
    for (i = sample_start; i < sample_end; i++) {
        if (avl_update(avl, records[i].student_id, records[i].course_id, 80) == OK)
            update_count++;
    }
    t_end = get_time_ms();
    avl_update_ms = t_end - t_start;
    printf("AVL树: %ld ms (成功 %d 次)\n", avl_update_ms, update_count);

    update_count = 0;
    t_start = get_time_ms();
    for (i = sample_start; i < sample_end; i++) {
        if (hash_update(hash, records[i].student_id, records[i].course_id, 80) == OK)
            update_count++;
    }
    t_end = get_time_ms();
    hash_update_ms = t_end - t_start;
    printf("哈希表: %ld ms (成功 %d 次)\n", hash_update_ms, update_count);

    /* 汇总表格 */
    printf("\n\n=== 性能汇总表（单位：ms） ===\n");
    printf("%-12s %-10s %-10s %-10s\n", "操作", "链表", "AVL树", "哈希表");
    printf("--------------------------------------------\n");
    printf("%-12s %-10ld %-10ld %-10ld\n", "插入(全量)", list_insert_ms, avl_insert_ms, hash_insert_ms);
    printf("%-12s %-10ld %-10ld %-10ld\n", "查找(100次)", list_find_ms, avl_find_ms, hash_find_ms);
    printf("%-12s %-10ld %-10ld %-10ld\n", "删除(100次)", list_delete_ms, avl_delete_ms, hash_delete_ms);
    printf("%-12s %-10ld %-10ld %-10ld\n", "修改(100次)", list_update_ms, avl_update_ms, hash_update_ms);

    /* 释放资源 */
    list_destroy(list);
    avl_destroy(avl);
    hash_destroy(hash);
    free_records(records);
}
