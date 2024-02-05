#include "rbtree.h"

#include <stdlib.h>
#include <assert.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil_node = malloc(sizeof(node_t));

  nil_node->color = RBTREE_BLACK;
  p->nil = nil_node;
  p->root = p->nil;
  return p;
}
node_t *new_node(rbtree *t, key_t key)
{
    node_t *n = malloc(sizeof(node_t));

    n->left = t->nil;
    n->right = t->nil;
    n->key = key;
    n->color = RBTREE_RED;
    return n;
}
void delete_node(node_t *n,rbtree *t){
  if(n == t->nil){
    return;
  }
  delete_node(n->left,t);
  delete_node(n->right,t);
  free(n);
  n = NULL;
  return;
}
void delete_rbtree(rbtree *t) {
  if(t == NULL){
    return;
  }
  delete_node(t->root,t);
  free(t->nil);
  t->nil = NULL;
  free(t);
  t = NULL;
  return;
}
void left_rotation(rbtree *t,node_t *x)
{
    //x의 오른쪽 자식 노드 y 선언 
    node_t *y = x->right;
    //y의 왼쪽 자식노드를 x의 오른쪽 자식노드로 변경 
    x->right = y->left;
    //y의 자식노드가 NIL이 아니면, y의 왼쪽 자식노드의 부모를 x로 변경
    if (y->left != t->nil)
        y->left->parent = x;
    //y의 부모노드를 x의 부모로 변경
    y->parent = x->parent;
    //x is root
    if(x->parent == t->nil)
        t->root = y;
    // x is left child
    else if(x == x->parent->left)
        x->parent->left = y;
    else //x is right child
        x->parent->right = y;
    // y의 왼쪽 자식을 x로 
    y->left = x;
    // x의 부모를 y로 
    x->parent = y;
}
void right_rotation(rbtree *t,node_t *x)
{
    node_t *y = x->left;
    x->left = y->right;
    if(y->right != t->nil)
        y->right->parent = x;
    y->parent = x->parent;
    //x is root
    if(x->parent == t->nil)
        t->root = y;
    // x is left child 
    else if (x==x->parent->left)
        x->parent->left = y;
    else // x is right child
        x->parent->right = y;
    y->right = x;
    x->parent = y;
}
void insert_fixup(rbtree *t,node_t *z)
{
    while(z->parent->color == RBTREE_RED)
    {
        if (z->parent == z->parent->left) // z.parent is left child
        {
            node_t *y = z->parent->parent->right; // uncle of z
            if (y->color == RBTREE_RED) // case 1 
            {
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            }else // case 2 or 3
            {
                if (z== z->parent->right) // case 2 
                {
                    z = z->parent; // marked z.parent as new z
                    left_rotation(t,z);
                }
                // case 3 
                z->parent->color = RBTREE_BLACK; //made parent black
                z->parent->parent->color = RBTREE_RED; // made grandparent red
                right_rotation(t,z->parent->parent);
            }
        }else // z.parent is right child
        {
            node_t *y = z->parent->parent->left; // uncle of z 
            if (y->color == RBTREE_RED) // case 1 
            {
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            }else
            {
                if (z==z->parent->left)
                {
                    z = z->parent;
                    right_rotation(t,z);
                }
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                left_rotation(t,z->parent->parent);
            }
        }
    }
    // tree의 root를 black 으로
    t->root->color = RBTREE_BLACK;
}
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *y = t->nil;
  node_t *n = new_node(t,key);
    node_t *temp = t->root;

    while(temp != t->nil)
    {
        y = temp;
        if(n->key < temp->key)
            temp = temp->left;
        else
            temp = temp->right;
    }
    n->parent = y;
    if(y == t->nil)
        t->root = n;
    else if (n->key < y->key)
        y->left = n;
    else
        y->right = n;
    n->left = t->nil;
    n->right = t->nil;
    n->color = RBTREE_RED;
    insert_fixup(t,n);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *x = t->root;
  while(x!= t->nil && key != x->key){
    if(x->key < key){
      x = x->right;
    }else{
      x = x->left;
    }
  }
  if(x== t->nil) {
    return NULL;
  }
  return x;
}
node_t* minimum(rbtree *t, node_t *x){
    while(x->left != t->nil)
        x = x->left;
    return x;
}
node_t *rbtree_min(const rbtree *t) {
  node_t *x = t->root;
  while(x->left != t->nil)
        x = x->left;
  return x;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *x = t->root;
  while(x->right != t->nil)
        x = x->right;
  return x;
}
void transplant(rbtree *t, node_t *u, node_t *v)
{
    if(u->parent == t->nil) // u is root
        t->root = v;
    else if(u == u->parent->left) // u is left child
        u->parent->left = v;
    else // u is right child
        u->parent->right = v;
    v->parent = u->parent;
}
void delete_fixup(rbtree *t, node_t *x)
{
    while (x!= t->root && x->color == RBTREE_BLACK)
    {
        if(x == x->parent->left)
        {
            node_t *w = x->parent->right;
            if(w->color == RBTREE_RED) // case 1
            {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                left_rotation(t,x->parent);
                w = x->parent->right;
            }
            if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)//case 2
            {
                w->color = RBTREE_RED;
                x = x->parent;
            }else //case 3 or 4
            {
                if(w->right->color == RBTREE_BLACK) //case 3 
                {
                    w->left->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    right_rotation(t,w);
                    w = x->parent->right;
                }//case 4
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->right->color = RBTREE_BLACK;
                left_rotation(t,x->parent);
                x= t->root;
            }
        }else{
            node_t *w = x->parent->left;
            if(w->color ==RBTREE_RED) //case 1 
            {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                right_rotation(t,x->parent);
                w = x->parent->left;
            }
            if(w->right->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK ) //case 2 
            {
                w->color = RBTREE_RED;
                x= x->parent;
            }else //case 3 or 4
            {
                if(w->left->color == RBTREE_BLACK) //case 3
                {
                    w->right->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    left_rotation(t,w);
                    w = w->parent->left;
                }//case 4
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->left->color = RBTREE_BLACK;
                right_rotation(t,x->parent);
                x = t->root;
            }
        }
    }
    x->color = RBTREE_BLACK;
}
int rbtree_erase(rbtree *t, node_t *p) {
  node_t *y = p;
  node_t *x;
  color_t y_origin_color = y->color;
  
  if(p->left == t->nil) // no children or only right
  {
      x = p->right;
      transplant(t,p,p->right);
  }else if (p->right == t->nil) // only left
  {
      x = p->left;
      transplant(t,p,p->left);
  }else { //both children
      y = minimum(t,p->right);
      y_origin_color = y->color;
      x = y->right;
      if (y->parent == p) // y가 z의 자식
      {
          x->parent = p;
      }else{
          transplant(t,y,y->right);
          y->right = p->right;
          y->right->parent = y;
      }
      transplant(t,p,y);
      y->left = p->left;
      y->left->parent = y;
      y->color = p->color;
  }
  if(y_origin_color == RBTREE_BLACK)
  {
      delete_fixup(t,x);
  }
  free(p);
  return 0;
}
int inorder_array(node_t *nil,node_t *root,key_t *arr,const size_t n, int index){
  if(root == nil){
    return index;
  }
  if(index == n){
    return index;
  }
  index = inorder_array(nil,root->left,arr,n,index);
  if(index<n){
    arr[index++] = root->key;
  }
  index= inorder_array(nil,root->right,arr,n,index);
  return index;
}
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  if(t->root == t->nil){
    return 0;
  }
  inorder_array(t->nil,t->root,arr,n,0);
  return 0;
}