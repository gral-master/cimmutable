/************************************************************************
 * A big part of this code come from http://www.eternallyconfuzzled.com. *
 * Thanks to the author of this website for its sharing.                 *
 *                                                                       *
 * Copyright © 2016 Matthieu Dien <matthieu.dien@lip6.fr>                *
 * This work is free. You can redistribute it and/or modify it under the *
 * terms of the Do What The Fuck You Want To Public License, Version 2,  *
 * as published by Sam Hocevar. See the COPYING file for more details.   *
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "avl.h"

typedef struct _avl_node {
  struct _data_t* data;
  int ref_count;
  int balance;
  struct _avl_node* sons[2];
} avl_node;

struct _avl_tree {
  avl_node* root;
  int (*compare)(struct _data_t*, struct _data_t*);
};

/*******************
 *   Constructors   *
 *******************/

data_t* make_data(int content) {
  data_t* r = malloc(sizeof(*r));
  r->content = content;
  return r;
}

avl_node* make_node(data_t* data) {
  avl_node* r = malloc(sizeof(*r));

  r->data = data;
  r->ref_count = 1;
  r->balance = 0;
  r->sons[0] = r->sons[1] = NULL;
  return r;
}

avl_tree* avl_make_empty_tree(int (*compare)(struct _data_t*, struct _data_t*)) {
  avl_tree* r = malloc(sizeof(*r));
  r->root = NULL;
  r->compare = compare;
  return r;
}

avl_node* avl_copy_node(avl_node* node) {
  if (node) {
    avl_node* new = malloc(sizeof(*new));
    new->data = node->data;
    new->ref_count = 1;
    new->balance = node->balance;
    new->sons[0] = node->sons[0];
    new->sons[1] = node->sons[1];
    if (new->sons[0]) new->sons[0]->ref_count++;
    if (new->sons[1]) new->sons[1]->ref_count++;
    return new;
  } else {
    return NULL;
  }
}

avl_tree* avl_copy_tree(avl_tree* tree) {
  if (tree) {
    avl_tree* new = malloc(sizeof(*new));
    if ((new->root = tree->root) != NULL) {
      new->root->ref_count++;
    }
    return new;
  } else {
    return NULL;
  }
}

/*******************
 *    Destructor    *
 *******************/

void erase_tree(avl_node* root) {

  if (root && --root->ref_count == 0) {
    avl_node* l = root->sons[0], *r = root->sons[1];

    free(root->data);
    free(root);
    erase_tree(l);
    erase_tree(r);
  }
}

void avl_erase_tree(avl_tree* tree) {
  erase_tree(tree->root);
  free(tree);
}

/*******************
 *      Search      *
 *******************/

data_t* search_r(avl_node* root, data_t* data,
		 int (*compare)(struct _data_t*, struct _data_t*)) {
  if (root == NULL)
    return NULL;
  else
    if ((*compare)(data, root->data) == 0)
      return root->data;
    else
      {
	int dir = (*compare)(root->data, data) < 0;
        return search_r(root->sons[dir], data, compare);
      }
}

data_t* avl_search(avl_tree* tree, data_t* data) {
  return search_r(tree->root, data, tree->compare);
}


/*******************
 *     Rotation     *
 *******************/

// dir == 0 means left rotation and 1 means right
avl_node* single_rotation(avl_node* root, int dir)
{
  avl_node* save = root->sons[!dir];

  root->sons[!dir] = save->sons[dir];
  save->sons[dir] = root;

  return save;
}

// dir == 0 means right-left and 1 means left-right
avl_node* double_rotation(avl_node* root, int dir)
{
  root->sons[!dir] = single_rotation(root->sons[!dir], !dir);
  return single_rotation(root, dir);
}




// dir == 0 means left rotation and 1 means right
avl_node* single_rotation_remove(avl_node* root, int dir)
{
  avl_node* save = avl_copy_node(root->sons[!dir]);

  root->sons[!dir] = save->sons[dir];
  save->sons[dir] = root;

  return save;
}

// dir == 0 means right-left and 1 means left-right
avl_node* double_rotation_remove(avl_node* root, int dir)
{
  root->sons[!dir] = single_rotation_remove(root->sons[!dir], !dir);
  return single_rotation(root, dir);
}


/* Adjust the balances before the application of double_rotation
   (right-left rotation on the following example)
   x
   / \
   A   y
   / \
   z   D
   / \
   B   C
*/
void adjust_balance(avl_node* x, int dir, int bal)
{
  avl_node* y = x->sons[dir];
  avl_node* z = y->sons[!dir];

  // Case: (A ,(x, +2), ((B, (z, 0), C), (y, -1), D))
  if (z->balance == 0)
    {
      x->balance = y->balance = 0;
    }
  // Case: (A ,(x, +2), ((B, (z, +1), C), (y, -1), D))
  else if (z->balance == bal)
    {
      x->balance = -bal;
      y->balance = 0;
    }
  // Case: (A ,(x, +2), ((B, (z, -1), C), (y, -1), D))
  else
    {
      x->balance = 0;
      y->balance = bal;
    }

  z->balance = 0;
}


/*******************
 *    Insertion     *
 *******************/

avl_node* insert_balance(avl_node* root, int dir)
{
  avl_node* n = root->sons[dir];
  int bal = dir == 0 ? -1 : +1;

  if (n->balance == bal)
    {
      root->balance = n->balance = 0;
      root = single_rotation(root, !dir);
    }
  else /* n->balance == -bal */
    {
      adjust_balance(root, dir, bal);
      root = double_rotation(root, !dir);
    }

  return root;
}

avl_node* insert_node(avl_node* root, data_t* data,
		      int (*compare)(struct _data_t*, struct _data_t*))
{
  /* Empty tree case */
  if(root == NULL)
    root = make_node(data);
  else
    {
      avl_node* head = make_node((void*)0); /* False tree root */
      avl_node *s, *t;     /* Place to rebalance and parent */
      avl_node *p, *q;     /* Iterator and save pointer */
      int dir;

      t = head;
      root = t->sons[1] = avl_copy_node(root);
      /* Search down the tree, saving rebalance points */
      for (s = p = t->sons[1];; p = q) {
	dir = (*compare)(p->data, data) < 0;
	if(p->sons[dir]) p->sons[dir]->ref_count--; // undo the increment done by avl_copy_node.
	q = p->sons[dir] = avl_copy_node(p->sons[dir]);
	
	if (q == NULL)
	  break;
	
	if (q->balance != 0) {
	  t = p;
	  s = q;
	}
      }
      
      /* Insert the new node */
      p->sons[dir] = q = make_node(data);

      /* Update balance factors */
      for (p = s; p != q; p = p->sons[dir])
        {
          dir = (*compare)(p->data, data) < 0;
          p->balance += dir == 0 ? -1 : +1;
        }

      q = s; /* Save rebalance point for parent fix */

      /* Rebalance if necessary */
      if (abs(s->balance) > 1)
        {
          dir = (*compare)(s->data, data) < 0;
          s = insert_balance(s, dir);
        }

      /* Fix parent */
      if (q == head->sons[1])
        root = s;
      else
        t->sons[q == t->sons[1]] = s;
    }

  return root;
}

avl_tree* avl_insert(avl_tree* tree, data_t* data)
{
  avl_node* root = insert_node(tree->root, data, tree->compare);

  avl_tree* new_tree = avl_make_empty_tree(tree->compare);
  new_tree->root = root;

  return new_tree;
}


/*******************
 *     Deletion     *
 *******************/

avl_node* remove_balance(avl_node* root, int dir, int *done)
{
  avl_node* n = root->sons[!dir];
  int bal = dir == 0 ? -1 : +1;

  if (n->balance == -bal)
    {
      root->balance = n->balance = 0;
      root = single_rotation_remove(root, dir);
    }
  else if (n->balance == bal)
    {
      adjust_balance(root, !dir, -bal);
      root = double_rotation_remove(root, dir);
    }
  else /* n->balance == 0 */
    {
      root->balance = -bal;
      n->balance = bal;
      root = single_rotation_remove(root, dir);
      *done = 1;
    }

  return root;
}

avl_node* remove_node(avl_node* root, data_t* data, int* done,
		      int (*compare)(struct _data_t*, struct _data_t*))
{
  if (root != NULL)
    {
      int dir;
      root = avl_copy_node(root);

      /* Remove node */
      if ((*compare)(root->data, data) == 0)
        {
          /* Unsons and fix parent */
          if (root->sons[0] == NULL || root->sons[1] == NULL)
            {
              dir = root->sons[0] == NULL;

              return avl_copy_node(root->sons[dir]);
            }
          else
            {
              /* Find inorder predecessor */
              avl_node* heir = root->sons[0];

              while (heir->sons[1] != NULL) {
		heir = heir->sons[1];
	      }

              /* Copy and set new search data */
	      root->data = heir->data;
              data = heir->data;
            }
        }

      dir = (*compare)(root->data, data) < 0;
      if(root->sons[dir]) root->sons[dir]->ref_count--;
      root->sons[dir] = remove_node(root->sons[dir], data, done, compare);



      if (!*done)
        {
          /* Update balance factors */
          root->balance += dir != 0 ? -1 : +1;

          /* Terminate or rebalance as necessary */
          if (abs(root->balance) == 1)
            *done = 1;
          else if (abs(root->balance) > 1)
            root = remove_balance(root, dir, done);
        }
    }
  else
    *done = 1;


  return root;
}

avl_tree* avl_remove(avl_tree* tree, data_t* data)
{
  int done = 0;

  avl_tree* new_tree = avl_make_empty_tree(tree->compare);
  new_tree->root = remove_node(tree->root, data, &done, tree->compare);

  return new_tree;
}

/***********************
 * Invariants helpers  *
 ***********************/
#define MAX(x,y) x < y ? y : x

int depth_node (avl_node* node) {
  if (node) {
    return 1 + MAX(depth_node(node->sons[0]),depth_node(node->sons[1]));
  } else {
    return 0;
  }
}

int depth (avl_tree* tree) {
  return depth_node(tree->root);
}


/*******************
 *      Debug       *
 *******************/

void traverse(avl_node* root)
{
  if(root != NULL)
    {
      traverse(root->sons[0]);
      printf("%d ", root->data->content);
      traverse(root->sons[1]);
    }
}

void avl_traverse_and_print(avl_tree* tree)
{
  printf("[ ");
  traverse(tree->root);
  printf("]\n");
}

void avl_print_aux(avl_node* node, int tab){
  if(node){
    printf("%*d\n", tab*4, node->data->content);
    avl_print_aux(node->sons[0], tab+1);
    avl_print_aux(node->sons[1], tab+1);
  }else{
    printf("%*s\n", tab*4, "-");
  }
}

void avl_print(avl_tree* tree){
  avl_print_aux(tree->root, 0);
}
