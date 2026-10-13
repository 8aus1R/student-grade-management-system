#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "common.h"
#include "linked_list.h"
#include <sys/stat.h>

#ifdef _WIN32
  #include <direct.h>
#endif

/* ��CSV�ļ��������ݵ����� */
int load_from_csv(LinkedList* list, const char* filename);

/* ���������ݱ��浽CSV�ļ� */
int save_to_csv(LinkedList* list, const char* filename);

/* ȷ��Ŀ¼���ڣ��������򴴽� */
void ensure_directory(const char* dir);

/* ����ɸѡ���������ļ�������Ŀ¼·���������д��buf */
void generate_filter_filename(const char* dir,
                              const char* criteria_desc,
                              char* buf, int buf_size);

#endif
