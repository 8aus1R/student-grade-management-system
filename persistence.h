#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "common.h"
#include "linked_list.h"
#include <sys/stat.h>

#ifdef _WIN32
  #include <direct.h>
#endif

/* 从CSV文件加载数据到链表 */
int load_from_csv(LinkedList* list, const char* filename);

/* 将链表数据保存到CSV文件 */
int save_to_csv(LinkedList* list, const char* filename);

/* 确保目录存在，不存在则创建 */
void ensure_directory(const char* dir);

/* 根据筛选条件生成文件名（含目录路径），结果写入buf */
void generate_filter_filename(const char* dir,
                              const char* criteria_desc,
                              char* buf, int buf_size);

#endif
