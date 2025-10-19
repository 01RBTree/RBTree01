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

node_t* rbtree_insert (rbtree *t, const key_t key) 
{
  node_t* ptr = (t -> root);
  node_t* new_node = (node_t*)malloc(sizeof(node_t));

  
  // 새 노드 할당 실패 시 강제 종료
  assert(new_node != NULL);

  
  new_node -> color = RBTREE_RED;
  new_node -> key = key;
  new_node -> left = t -> nil;
  new_node -> right = t -> nil;
  new_node -> parent = t -> nil;


    // 트리의 루트 노드가 없으면 생성된 노드를 루트 노드로 생성 
  if(t -> root == t -> nil)
  {
    new_node -> color = RBTREE_BLACK;
    t -> root = new_node;
    return t -> root;
  }

  // 트리의 루트 노드가 있으면 BST 특징과 동일하게 알맞는 위치로 삽입
  while(1)
  {
    if( ptr ->key >= new_node -> key && ptr -> left == t -> nil)
    {
      ptr -> left = new_node;
      new_node -> parent = ptr; 
      break;
    }

    else if( ptr ->key < new_node -> key && ptr -> right == t -> nil)
    {
      ptr -> right = new_node;
      new_node -> parent = ptr; 
      break;
    }
  
    else if(ptr ->key >= new_node -> key && ptr -> left != t -> nil)
    ptr = ptr -> left;

    else if( ptr ->key < new_node -> key && ptr -> right != t -> nil)
    ptr = ptr -> right;

  }

  node_t* uncle = NULL; // 속성 위반 확인전 case에 활용될 삼촌 노드 포인터

  // 삽입된 노드가 속성 위반 검사
  while(new_node -> parent -> color == RBTREE_RED) // 삽입된 노드의 부모 색깔이 빨강인가? 맞다면 속성 4번 위반 
  {

    if(new_node -> parent == new_node -> parent -> parent -> left) // 부모 노드가 할아버지 노드의 왼쪽 자녀라면
    {
        uncle = new_node -> parent -> parent -> right; // 삼촌 노드는 할아버지 노드의 오른쪽 자녀가 됨

        if(uncle -> color == RBTREE_RED) // 삼촌 노드가 빨강이면 여기에서 Case 1 해결 시작 
        // 삼촌, 부모를 검정으로 바꾸고 할아버지는 빨강으로 바꾼다.
        {
          new_node -> parent -> color = RBTREE_BLACK; 
          uncle -> color = RBTREE_BLACK;
          new_node -> parent -> parent -> color = RBTREE_RED;
          new_node = new_node -> parent -> parent; // 할아버지 노드에서 다시 속성 위반을 검사해야함으로 포인터 주소를 할아버지 노드로 변경.
        }
        else // 삼촌 노드가 검정이면  여기에서 Case 2, 3으로 나뉨
        {

          if(new_node == new_node -> parent -> right) // 삽입된 노드가 부모 노드의 오른쪽 자식이면 Case 2번임
          // 삽입된 노드 주소를 부모 노드로 변경한 뒤 왼쪽으로 50도 회전
          {
            new_node = new_node -> parent;
            Left_Rotate(t, new_node);
          }

          // Case 2를 Case 3으로 바꿨으니 마지막 
          new_node -> parent -> color = RBTREE_BLACK; 
          new_node -> parent -> parent -> color = RBTREE_RED;
          Right_Rotate(t, new_node -> parent -> parent);
        }

    }
    else
    {
      uncle = new_node -> parent -> parent -> left;

      if(uncle -> color == RBTREE_RED)
      {
          new_node -> parent -> color = RBTREE_BLACK;
          uncle -> color = RBTREE_BLACK;
          new_node -> parent -> parent -> color = RBTREE_RED;
          new_node = new_node -> parent -> parent;
      }
      else
      {

         if(new_node == new_node -> parent -> left) // 부모의 왼쪽 자식이면 Case 2임
          {
            // Case 2 해결 진행
            new_node = new_node -> parent;
            Right_Rotate(t, new_node);
          }

          // Case 3 해결 진행
          new_node -> parent -> color = RBTREE_BLACK; 
          new_node -> parent -> parent -> color = RBTREE_RED;
          Left_Rotate(t, new_node -> parent -> parent);

        }
      }
      // 루트 노드는 항상 검정이어야 함으로 언제 루트 노드를 건들지 모르기 때문에 선언해주기
      t -> root -> color = RBTREE_BLACK;
    }
  
    return t -> root;
  }

  node_t *rbtree_find(const rbtree *t, const key_t key) 
{  
  node_t* ptr = t -> root;

  if(ptr == t -> nil)
  return NULL;

 while(ptr != t -> nil)
 {
    if(ptr -> key == key)
    return ptr;
    else
    {
      if(ptr -> key > key)
      ptr = ptr -> left;
      else
      ptr = ptr -> right;
    }
 }

  return NULL;
}