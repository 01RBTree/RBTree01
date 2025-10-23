#include <stdlib.h>
#include "rbtree.h"

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

node_t* grandparent(node_t* node) {
    if (node && node->parent)
        return node->parent->parent;
    else
        return NULL;
}

node_t* uncle(node_t* node) {
    node_t* g = grandparent(node);

    if (!g) return NULL;
    if (g->parent == g->left)
        return g->right;
    else
        return g->left;
}

// #1. 루트노드는 BLACK
void follow_rule_1(node_t* n, rbtree* t) {
    if (n->parent == NULL)
        n->color = RBTREE_BLACK;
    else
        follow_rule_2(n, t);
}
// #2. 모든노드 RED / BLACK
void follow_rule_2(node_t* n, rbtree* t) {
    if (n->parent->color == RBTREE_BLACK)
        return;
    else
        follow_rule_3(n, t);
}
// #3. 노드가 RED라면 자녀는 BLACK (RED는 연속하면 안됨)
// 부모 노드와 삼촌 노드가 모두 RED
void follow_rule_3(node_t* n, rbtree* t) {
    node_t *u = uncle(n), *g;

    if (u && u->color == RBTREE_RED) {
        u->color = RBTREE_BLACK;
        n->parent->color = RBTREE_BLACK;
        g = grandparent(n);
        g->color = RBTREE_RED;
        follow_rule_1(g, t);  // 바뀌어버린 할아버지로 재귀적 조정
    } else
        follow_rule_4(n, t);
}

// #4. 노드가 RED 면, 자녀는 BLACK (RED는 연속하면 안됨)
// 부모: 붉은색(follow_rule_2에서 이미 확인)
// 삼촌: 검은색(follow_rule_3에서 이미 확인)
// 부모가 조부의 왼쪽 && 노드는 부모의 오른쪽 => 왼쪽 회전해서 쭉 펼치기
// 부모가 조부의 오른쪽 && 노드는 부모의 왼쪽 => 오른쪽 회전해서 쭉 펼치기
void rotate_left(node_t* n, rbtree* t) {
    node_t* c = n->right;
    node_t* p = n->parent;

    if (c->left != NULL) c->left->parent = n;

    n->right = c->left;
    n->parent = c;
    c->left = n;
    c->parent = p;

    if (p) {
        if (p->left == n)
            p->left = c;
        else
            p->right = c;
    } else
        t->root = c;
}
void rotate_right(node_t* n, rbtree* t) {
    node_t* c = n->left;
    node_t* p = n->parent;

    if (c->right != NULL) c->right->parent = n;

    n->left = c->right;
    n->parent = c;
    c->right = n;
    c->parent = p;

    if (p) {
        if (p->left == n)
            p->left = c;
        else
            p->right = c;
    } else
        t->root = c;
}

void follow_rule_4(node_t* n, rbtree* t) {
    node_t* g = grandparent(n);

    if (n->parent == g->left && n == n->parent->right) {
        rotate_left(n->parent, t);
        n = n->left;
    } else if (n->parent == g->right && n == n->parent->left) {
        rotate_right(n->parent, t);
        n = n->right;
    }

    follow_rule_5(n, t);
}

void follow_rule_5(node_t* n, rbtree* t) {
    node_t* g = grandparent(n);

    n->parent->color = RBTREE_BLACK;
    g->color = RBTREE_RED;
    if (n == n->parent->left)
        rotate_right(g, t);  // 이게 되네?
    else
        rotate_left(g, t);
}

node_t* rbtree_min_from_node(node_t* root) {
    node_t* curNode = root;
    if (!curNode) return NULL;
    while (curNode->left) curNode = curNode->left;
    return curNode;
}

void rbtree_erase_fixup(rbtree* t, node_t* child, node_t* parent) {
    while (child != t->root &&
           (child == NULL || child->color == RBTREE_BLACK)) {
        // 1. child가 왼쪽 자식인 경우
        if (child == parent->left) {
            node_t* sibling = parent->right;
            // Case 1: 형제(sibling)가 RED
            if (sibling && sibling->color == RBTREE_RED) {
                sibling->color = RBTREE_BLACK;
                parent->color = RBTREE_RED;
                rotate_left(parent, t);
                sibling = parent->right;  // sibling 갱신
            }
            // Case 2: 형제와 형제의 두 자식 모두 BLACK
            // leaf node인 경우도 포함
            if (sibling &&
                (!sibling->left || sibling->left->color == RBTREE_BLACK) &&
                (!sibling->right || sibling->right->color == RBTREE_BLACK)) {
                sibling->color = RBTREE_RED;
                child = parent;  // child 갱신
                parent = parent->parent;
                continue;
            }
            // Case 3: 형제가 BLACK, 왼쪽만 RED, 오른쪽은 BLACK(leaf node okay)
            if (sibling && sibling->color == RBTREE_BLACK && sibling->left &&
                sibling->left->color == RBTREE_RED &&
                (!sibling->right || sibling->right->color == RBTREE_BLACK)) {
                sibling->left->color = RBTREE_BLACK;
                sibling->color = RBTREE_RED;
                rotate_right(sibling, t);
                sibling = parent->right;  // sibling 갱신
            }
            // Case 4: 형제가 BLACK, 오른쪽 자식이 RED
            if (sibling && sibling->color == RBTREE_BLACK && sibling->right &&
                sibling->right->color == RBTREE_RED) {
                sibling->color = parent->color;
                parent->color = RBTREE_BLACK;
                sibling->right->color = RBTREE_BLACK;
                rotate_left(parent, t);
                child = t->root;  // child를 root로
                break;
            }
        } else {
            // 오른쪽 자식인 경우는 좌우 대칭으로
            node_t* sibling = parent->left;
            // Case 1:
            if (sibling && sibling->color == RBTREE_RED) {
                sibling->color = RBTREE_BLACK;
                parent->color = RBTREE_RED;
                rotate_right(parent, t);
                sibling = parent->left;
            }
            // Case 2:
            if (sibling &&
                (!sibling->left || sibling->left->color == RBTREE_BLACK) &&
                (!sibling->right || sibling->right->color == RBTREE_BLACK)) {
                sibling->color = RBTREE_RED;
                child = parent;
                parent = parent->parent;
                continue;
            }
            // Case 3:
            if (sibling && sibling->color == RBTREE_BLACK && sibling->right &&
                sibling->right->color == RBTREE_RED &&
                (!sibling->left || sibling->left->color == RBTREE_BLACK)) {
                sibling->right->color = RBTREE_BLACK;
                sibling->color = RBTREE_RED;
                rotate_left(sibling, t);
                sibling = parent->left;
            }
            // Case 4:
            if (sibling && sibling->color == RBTREE_BLACK && sibling->left &&
                sibling->left->color == RBTREE_RED) {
                sibling->color = parent->color;
                parent->color = RBTREE_BLACK;
                sibling->left->color = RBTREE_BLACK;
                rotate_right(parent, t);
                child = t->root;
                break;
            }
        }
    }

    if (child) child->color = RBTREE_BLACK;
}
