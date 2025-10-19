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