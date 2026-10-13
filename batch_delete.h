#ifndef BATCH_DELETE_H
#define BATCH_DELETE_H

#include "common.h"
#include "linked_list.h"

int count_expired_records(LinkedList* list, const char* base_date, int years);
int batch_delete_expired(LinkedList* list, const char* base_date, int years);

#endif
