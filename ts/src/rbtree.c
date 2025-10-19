#include "rbtree.h"
#include <stdlib.h>
#include <assert.h>

rbtree* new_rbtree (void) 
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree)); 
  node_t * Sentinel_Nil = (node_t *)malloc(sizeof(node_t)); // 센티널 노드 선언

  if(p == NULL || Sentinel_Nil == NULL) // 둘 중 하나라도 메모리 할당 실패 시 강제 종료
  assert(0);

  // 센티널 노드 초기화
  Sentinel_Nil -> color = RBTREE_BLACK;
  Sentinel_Nil-> key = -1;
  Sentinel_Nil-> left = Sentinel_Nil;
  Sentinel_Nil-> right = Sentinel_Nil;
  Sentinel_Nil-> parent = Sentinel_Nil;
   
 // 할당한 RB tree 초기화
  p->nil = Sentinel_Nil;
  p->root = p -> nil;
  
  return p;
 
}

void Left_Rotate (rbtree* t, node_t* node)
{
  node_t* y = node -> right;
  node -> right = y -> left;

  if(y -> left != t -> nil)
  y -> left -> parent = node;

  y -> parent = node -> parent;

  if(node -> parent == t -> nil)
  t -> root = y;
  else if(node == node -> parent -> left)
  node -> parent -> left = y;
  else
  node -> parent -> right = y;

  y -> left = node;
  node -> parent = y;
}

void Right_Rotate (rbtree* t, node_t* node)
{
  node_t* y = node -> left;
  node -> left = y -> right;

  if(y -> right != t -> nil)
  y -> right -> parent = node;

  y -> parent = node -> parent;

  if(node -> parent == t -> nil)
  t -> root = y;
  else if(node == node -> parent -> right)
  node -> parent -> right = y;
  else
  node -> parent -> left = y;

  y -> right = node;
  node -> parent = y;
}