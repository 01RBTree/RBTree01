#include "rbtree.h"

#include <stdlib.h>

rbtree* new_rbtree(void) {
    rbtree* p = (rbtree*)calloc(1, sizeof(rbtree));
    // MEMO: calloc은 조명하는 메모리 공간의 모든 비트를 0으로 초기화한다.
    // 해당 영역의 데이터를 읽는 관점에 따라 0이고,
    // 주소값으로 해석한다면 NULL이다.
    // 그래서 테스트에 통과한다.
    // TODO: initialize struct if needed
    return p;
}

void delete_rbtree(rbtree* t) {
    // TODO: reclaim the tree nodes's memory
    free(t);
}

node_t* rbtree_insert(rbtree* t, const key_t key) {
    // PSEUDO_CODE:: if empty tree
    if (!t->root) {
        t->root = new_node(key);
        t->root->color = RBTREE_BLACK;
        return t->root;
    }

    // PSEUDO_CODE:: find and insert (right-child if same key)
    node_t* parent = t->root;
    child_side_t childSide = RIGHT_CHILD;
    while (parent) {
        if (parent->key <= key) {  // right subtree
            if (!parent->right) {  // right side empty?
                childSide = RIGHT_CHILD;
                break;
            }
            parent = parent->right;
        } else {  // 좌측이동
            if (!parent->left) {
                childSide = LEFT_CHILD;
                break;
            }
            parent = parent->left;
        }
    }
    node_t* newNode = create_child(parent, new_node(key), childSide);

    // PSEUDO_CODE:: 조건 확인 & 조건 대응
    follow_rule_1(newNode, t);

    return newNode;
}

node_t* rbtree_find(const rbtree* t, const key_t key) {
    node_t* curNode = t->root;

    while (curNode) {
        if (curNode->key == key)
            return curNode;
        else if (key < curNode->key)
            curNode = curNode->left;
        else
            curNode = curNode->right;
    }
    return NULL;
}

node_t* rbtree_min(const rbtree* t) {
    node_t* curNode = t->root;
    if (!curNode) return NULL;
    while (curNode->left) curNode = curNode->left;
    return curNode;
}

node_t* rbtree_max(const rbtree* t) {
    node_t* curNode = t->root;
    if (!curNode) return NULL;
    while (curNode->right) curNode = curNode->right;
    return curNode;
}

int rbtree_erase(rbtree* t, node_t* p) {
    // TODO: implement erase
    return 0;
}

int rbtree_to_array(const rbtree* t, key_t* arr, const size_t n) {
    // TODO: implement to_array
    return 0;
}
