#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;
typedef enum { LEFT_CHILD, RIGHT_CHILD } child_side_t;

typedef int key_t;

typedef struct node_t {
    color_t color;
    key_t key;
    struct node_t *parent, *left, *right;
} node_t;

typedef struct {
    node_t* root;
    node_t* nil;  // for sentinel
} rbtree;

rbtree* new_rbtree(void);
void delete_rbtree(rbtree*);

node_t* rbtree_insert(rbtree*, const key_t);
node_t* rbtree_find(const rbtree*, const key_t);
node_t* rbtree_min(const rbtree*);
node_t* rbtree_max(const rbtree*);
int rbtree_erase(rbtree*, node_t*);

int rbtree_to_array(const rbtree*, key_t*, const size_t);

// utils
node_t* new_node(const key_t);
node_t* create_child(node_t*, node_t*, child_side_t);
node_t* grandparent(node_t*);
node_t* uncle(node_t*);
void rotate_left(node_t*, rbtree*);
void rotate_right(node_t*, rbtree*);
void follow_rule_1(node_t*, rbtree*);
void follow_rule_2(node_t*, rbtree*);
void follow_rule_3(node_t*, rbtree*);
void follow_rule_4(node_t*, rbtree*);
void follow_rule_5(node_t*, rbtree*);

#endif  // _RBTREE_H_
