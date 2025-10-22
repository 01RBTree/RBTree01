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

node_t* new_node(const key_t key) {
    node_t* newNode = (node_t*)malloc(sizeof(node_t));
    newNode->color = RBTREE_RED;
    newNode->key = key;
    newNode->parent = NULL;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

node_t* create_child(node_t* parent, node_t* new_node, child_side_t side) {
    new_node->parent = parent;
    if (side == LEFT_CHILD)
        parent->left = new_node;
    else
        parent->right = new_node;

    return new_node;
}

node_t* rbtree_insert(rbtree* t, const key_t key) {
    if (!t->root) {
        t->root = new_node(key);
        return t->root;
    }
    // PSEUDO_CODE:
    // 1. 자리 찾기
    //      - 동일한 값은 오른쪽 자식으로 자리잡는다.
    // 2. 조건 확인
    // 3. 조건 대응

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
    return create_child(parent, new_node(key), child_side);
}

node_t* rbtree_find(const rbtree* t, const key_t key) {
    node_t* curNode = t->root;

    if (curNode->key == key)
        return curNode;
    else {
        do {
            if (curNode->key > key && curNode->left)
                curNode = curNode->left;
            else if (curNode->key < key && curNode->right)
                curNode = curNode->right;
            else
                return NULL;
        } while (curNode->key != key);
        return curNode;
    }
}

node_t* rbtree_min(const rbtree* t) {
    // TODO: implement find
    return t->root;
}

node_t* rbtree_max(const rbtree* t) {
    // TODO: implement find
    return t->root;
}

int rbtree_erase(rbtree* t, node_t* p) {
    // TODO: implement erase
    return 0;
}

int rbtree_to_array(const rbtree* t, key_t* arr, const size_t n) {
    // TODO: implement to_array
    return 0;
}
