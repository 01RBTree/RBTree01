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
    if (!p) return -1;

    node_t* to_delete = p;

    // Case 1: Node has two children
    if (p->left && p->right) {
        node_t* successor = rbtree_min_from_node(p->right);
        // Swap key and color
        key_t temp_key = p->key;
        color_t temp_color = p->color;
        p->key = successor->key;
        p->color = successor->color;
        successor->key = temp_key;
        successor->color = temp_color;
        to_delete = successor;  // narrow down to one-child case
    }

    // subtree의 최소 또는 최대값 골랐으니 이제 어차피 하나 남음
    // 둘다 없는 경우 알아서 NULL
    node_t* child = to_delete->left ? to_delete->left : to_delete->right;
    color_t original_color = to_delete->color;

    // 삭제: 부모 -> 자식 연결
    if (to_delete->parent) {
        if (to_delete == to_delete->parent->left)
            to_delete->parent->left = child;
        else
            to_delete->parent->right = child;
    } else {
        t->root = child;
    }
    // 삭제: 자식 -> 부모 연결
    if (child) child->parent = to_delete->parent;

    free(to_delete);

    // Rebalance if to_delete was black
    // PSEUDO_CODE:
    // 1. while (child != root && (child == NULL || child->color == BLACK)):
    //     - Case 1: 형제(sibling)가 RED
    //       - 부모를 RED로, 형제를 BLACK으로
    //       - 부모 기준으로 rotate
    //       - sibling을 새로 갱신
    //     - Case 2: 형제와 형제의 두 자식 모두 BLACK
    //       - 형제를 RED로
    //       - child를 부모로 올림
    //     - Case 3: 형제는 BLACK, 형제의 왼쪽 자식만 RED, 오른쪽 자식은
    // BLACK
    //       - 형제의 왼쪽 자식을 BLACK, 형제를 RED로
    //       - 형제 기준으로 rotate
    //       - sibling을 새로 갱신
    //     - Case 4: 형제는 BLACK, 형제의 오른쪽 자식이 RED
    //       - 형제의 색을 부모의 색으로
    //       - 부모와 형제의 오른쪽 자식을 BLACK으로
    //       - 부모 기준으로 rotate
    //       - child를 root로 설정 (종료)
    //
    // 2. child가 NULL이 아니면 BLACK으로 칠함

    if (original_color == RBTREE_BLACK)
        rbtree_erase_fixup(t, child, to_delete->parent);

    return 0;
}

int rbtree_to_array(const rbtree* t, key_t* arr, const size_t n) {
    // TODO: implement to_array
    return 0;
}
