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

typedef struct _avl_tree avl_tree;

typedef struct _data_t {
  int content;
} data_t;

data_t* make_data(int content);

avl_tree* avl_make_empty_tree(int (*compare)(struct _data_t*, struct _data_t*));

void avl_erase_tree(avl_tree* t);

data_t* avl_search(avl_tree* tree, data_t* data);

avl_tree* avl_insert(avl_tree* tree, data_t* data);

avl_tree* avl_remove(avl_tree* tree, data_t* data);

void avl_traverse_and_print(avl_tree* tree);

void avl_print(avl_tree* tree);

avl_tree* merge(avl_tree* tree1, avl_tree* tree2);


#endif
