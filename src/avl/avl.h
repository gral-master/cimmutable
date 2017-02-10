/************************************************************************
* A big part of this code come from http://www.eternallyconfuzzled.com. *
* Thanks to the author of this website for its sharing.                 *
*                                                                       *
* Copyright © 2016 Matthieu Dien <matthieu.dien@lip6.fr>                *
* This work is free. You can redistribute it and/or modify it under the *
* terms of the Do What The Fuck You Want To Public License, Version 2,  *
* as published by Sam Hocevar. See the COPYING file for more details.   *
************************************************************************/

#ifndef __AVL__
#define __AVL__


/***************************
 * The AVL Trees.
 ***************************/


typedef void avl_data_t;

typedef struct _avl_node {
  avl_data_t* data;
  int ref_count;
  int balance;
  struct _avl_node* sons[2];
} avl_node;

typedef struct _avl_tree {
  avl_node* root;
  int size;
  int (*compare)(avl_data_t*, avl_data_t*);
} avl_tree;

avl_tree* avl_make_empty_tree(int (*compare)(avl_data_t*, avl_data_t*));

void avl_erase_tree(avl_tree* t);

avl_data_t* avl_search(avl_tree* tree, avl_data_t* data);

avl_tree* avl_insert(avl_tree* tree, avl_data_t* data);
void avl_insert_mutable(avl_tree* tree, void* data); /* Careful with that one */

avl_tree* avl_remove(avl_tree* tree, avl_data_t* data, avl_data_t** ret_data);

void avl_print(avl_tree* tree, char* (*data_to_string)(avl_data_t*));

avl_tree* merge(avl_tree* tree1, avl_tree* tree2);


#endif
