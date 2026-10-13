#include "batch_delete.h"

static void get_cutoff_date(const char* base_date, int years, char* cutoff) {
    int y, m, d;
    sscanf(base_date, "%d-%d-%d", &y, &m, &d);
    y -= years;
    sprintf(cutoff, "%04d-%02d-%02d", y, m, d);
}

int count_expired_records(LinkedList* list, const char* base_date, int years) {
    char cutoff[DATE_LEN];
    ListNode* cur;
    int count = 0;

    if (!list || !base_date || !is_valid_date(base_date)) return 0;

    get_cutoff_date(base_date, years, cutoff);
    cur = list->head;
    while (cur) {
        if (date_compare(cur->data.select_date, cutoff) < 0) {
            count++;
        }
        cur = cur->next;
    }
    return count;
}

int batch_delete_expired(LinkedList* list, const char* base_date, int years) {
    char cutoff[DATE_LEN];
    ListNode *cur, *next;
    int count = 0;

    if (!list || !base_date || !is_valid_date(base_date)) return 0;

    get_cutoff_date(base_date, years, cutoff);
    cur = list->head;
    while (cur) {
        next = cur->next;
        if (date_compare(cur->data.select_date, cutoff) < 0) {
            if (cur->prev) cur->prev->next = cur->next;
            else list->head = cur->next;
            if (cur->next) cur->next->prev = cur->prev;
            else list->tail = cur->prev;

            free(cur);
            list->size--;
            count++;
        }
        cur = next;
    }
    return count;
}
