#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "common.h"

typedef struct AvlNode {
    Record data;
    struct AvlNode* left;
    struct AvlNode* right;
    int height;
} AvlNode;

typedef struct {
    AvlNode* root;
    int size;
} AvlTree;

AvlTree* avl_create();
void avl_destroy(AvlTree* tree);

int avl_insert(AvlTree* tree, const Record* rec);
int avl_delete(AvlTree* tree, const char* student_id, const char* course_id);
Record* avl_find(AvlTree* tree, const char* student_id, const char* course_id);
int avl_update(AvlTree* tree, const char* student_id, const char* course_id, int new_score);
void avl_traverse(AvlTree* tree, void (*visit)(const Record*));
int avl_size(AvlTree* tree);

#endif
