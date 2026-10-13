#include "avl_tree.h"

static int max(int a, int b) { return a > b ? a : b; }

static int node_height(AvlNode* node) {
    return node ? node->height : 0;
}

/* 右旋 */
static AvlNode* right_rotate(AvlNode* y) {
    AvlNode* x = y->left;
    AvlNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(node_height(y->left), node_height(y->right)) + 1;
    x->height = max(node_height(x->left), node_height(x->right)) + 1;
    return x;
}

/* 左旋 */
static AvlNode* left_rotate(AvlNode* x) {
    AvlNode* y = x->right;
    AvlNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(node_height(x->left), node_height(x->right)) + 1;
    y->height = max(node_height(y->left), node_height(y->right)) + 1;
    return y;
}

/* 获取节点平衡因子 */
static int get_balance(AvlNode* node) {
    return node ? node_height(node->left) - node_height(node->right) : 0;
}

/* 递归插入 */
static AvlNode* insert_node(AvlNode* node, const Record* rec, int* status) {
    int cmp;
    int balance;

    if (!node) {
        AvlNode* new_node = (AvlNode*)malloc(sizeof(AvlNode));
        if (!new_node) { *status = ERROR; return NULL; }
        new_node->data = *rec;
        new_node->left = new_node->right = NULL;
        new_node->height = 1;
        *status = OK;
        return new_node;
    }

    cmp = record_key_compare(rec, &node->data);
    if (cmp < 0) {
        node->left = insert_node(node->left, rec, status);
    } else if (cmp > 0) {
        node->right = insert_node(node->right, rec, status);
    } else {
        *status = DUPLICATE;
        return node;
    }

    /* 更新高度 */
    node->height = 1 + max(node_height(node->left), node_height(node->right));

    /* 平衡调整 */
    balance = get_balance(node);

    /* LL */
    if (balance > 1 && record_key_compare(rec, &node->left->data) < 0)
        return right_rotate(node);
    /* RR */
    if (balance < -1 && record_key_compare(rec, &node->right->data) > 0)
        return left_rotate(node);
    /* LR */
    if (balance > 1 && record_key_compare(rec, &node->left->data) > 0) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }
    /* RL */
    if (balance < -1 && record_key_compare(rec, &node->right->data) < 0) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

/* 寻找最小节点 */
static AvlNode* find_min_node(AvlNode* node) {
    AvlNode* cur = node;
    while (cur && cur->left) cur = cur->left;
    return cur;
}

/* 递归删除 */
static AvlNode* delete_node(AvlNode* node, const char* sid, const char* cid, int* status) {
    Record tmp;
    int cmp;
    int balance;

    if (!node) {
        *status = NOT_FOUND;
        return NULL;
    }

    strcpy(tmp.student_id, sid);
    strcpy(tmp.course_id, cid);
    cmp = record_key_compare(&tmp, &node->data);

    if (cmp < 0) {
        node->left = delete_node(node->left, sid, cid, status);
    } else if (cmp > 0) {
        node->right = delete_node(node->right, sid, cid, status);
    } else {
        /* 找到待删除节点 */
        if (!node->left || !node->right) {
            AvlNode* temp = node->left ? node->left : node->right;
            if (!temp) {
                temp = node;
                node = NULL;
            } else {
                *node = *temp; /* 复制内容 */
            }
            free(temp);
            *status = OK;
        } else {
            AvlNode* temp = find_min_node(node->right);
            node->data = temp->data;
            node->right = delete_node(node->right, temp->data.student_id, temp->data.course_id, status);
        }
    }

    if (!node) return node;

    /* 更新高度 */
    node->height = 1 + max(node_height(node->left), node_height(node->right));

    /* 平衡调整 */
    balance = get_balance(node);

    /* LL */
    if (balance > 1 && get_balance(node->left) >= 0)
        return right_rotate(node);
    /* LR */
    if (balance > 1 && get_balance(node->left) < 0) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }
    /* RR */
    if (balance < -1 && get_balance(node->right) <= 0)
        return left_rotate(node);
    /* RL */
    if (balance < -1 && get_balance(node->right) > 0) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

/* 递归查找 */
static Record* find_node(AvlNode* node, const char* sid, const char* cid) {
    Record tmp;
    int cmp;
    if (!node) return NULL;

    strcpy(tmp.student_id, sid);
    strcpy(tmp.course_id, cid);
    cmp = record_key_compare(&tmp, &node->data);

    if (cmp < 0) return find_node(node->left, sid, cid);
    if (cmp > 0) return find_node(node->right, sid, cid);
    return &node->data;
}

/* 中序遍历 */
static void in_order(AvlNode* node, void (*visit)(const Record*)) {
    if (!node) return;
    in_order(node->left, visit);
    visit(&node->data);
    in_order(node->right, visit);
}

/* 销毁树 */
static void destroy_tree(AvlNode* node) {
    if (!node) return;
    destroy_tree(node->left);
    destroy_tree(node->right);
    free(node);
}

AvlTree* avl_create() {
    AvlTree* tree = (AvlTree*)malloc(sizeof(AvlTree));
    if (!tree) return NULL;
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

void avl_destroy(AvlTree* tree) {
    if (!tree) return;
    destroy_tree(tree->root);
    free(tree);
}

int avl_insert(AvlTree* tree, const Record* rec) {
    int status = OK;
    if (!tree || !rec) return ERROR;
    tree->root = insert_node(tree->root, rec, &status);
    if (status == OK) tree->size++;
    return status;
}

int avl_delete(AvlTree* tree, const char* student_id, const char* course_id) {
    int status = OK;
    if (!tree || !student_id || !course_id) return ERROR;
    tree->root = delete_node(tree->root, student_id, course_id, &status);
    if (status == OK) tree->size--;
    return status;
}

Record* avl_find(AvlTree* tree, const char* student_id, const char* course_id) {
    if (!tree || !student_id || !course_id) return NULL;
    return find_node(tree->root, student_id, course_id);
}

/* 修改记录成绩 */
int avl_update(AvlTree* tree, const char* student_id, const char* course_id, int new_score) {
    Record* rec;
    if (!tree || !student_id || !course_id) return ERROR;
    rec = avl_find(tree, student_id, course_id);
    if (!rec) return NOT_FOUND;
    rec->score = new_score;
    return OK;
}

void avl_traverse(AvlTree* tree, void (*visit)(const Record*)) {
    if (!tree || !visit) return;
    in_order(tree->root, visit);
}

int avl_size(AvlTree* tree) {
    return tree ? tree->size : 0;
}
