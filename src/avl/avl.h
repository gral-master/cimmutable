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

/* avl_data_t and data_as_string **must** be defined on a 
   higher level of abstraction. */
typedef struct _avl_data_t avl_data_t;
char* avl_data_as_string (avl_data_t* data);

typedef struct _avl_tree avl_tree;

avl_tree* avl_make_empty_tree(int (*compare)(struct _avl_data_t*, struct _avl_data_t*));

void avl_erase_tree(avl_tree* t);

avl_data_t* avl_search(avl_tree* tree, avl_data_t* data);

avl_tree* avl_insert(avl_tree* tree, avl_data_t* data);

avl_tree* avl_remove(avl_tree* tree, avl_data_t* data);

void avl_traverse_and_print(avl_tree* tree);

void avl_print(avl_tree* tree);

avl_tree* merge(avl_tree* tree1, avl_tree* tree2);


#endif
