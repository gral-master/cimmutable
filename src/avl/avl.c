#include "avl.h"
#include <stdlib.h>
#include <stdio.h>


/* ********************************************** *\
              internal implementation
\* ********************************************** */

struct _imc_avl_tree_t {
  int balance;
  unsigned int ref_count;
  imc_data_t* data;
  struct _imc_avl_tree_t* sons[2];
};

/** \todo add ref_count in _imc_data_t ? */
struct _imc_data_t {
  int content;
};


/* ********************************************** *\
              internal implementation
\* ********************************************** */

/** 
 * A few defines to make avl easier to manipulate. 
 * \todo condider removing this two defines?
 */
#define AVL imc_avl_tree_t
#define DATA imc_data_t

/** The size of the indentation (used when dumping the avl) */
#define INDENT_SIZE 2

/** Prototype of the internal-only functions */
void unref_data (DATA* data);
void internal_avl_dump (FILE* fp_out, AVL* tree, unsigned int indent);


/* ********************************
   Creation and desctruction
*/
              
/**
 * Creates a node
 */
AVL* imc_avl_create (DATA* data) {
  AVL* tree = malloc(sizeof *tree);
  tree->balance = 0;
  tree->data = data;
  tree->ref_count = 1;
  tree->sons[0] = tree->sons[1] = NULL;
  return tree;
}

/**
 * Creates an avl
 */
AVL* imc_avl_create (DATA* data, AVL* left, AVL* right) {
  AVL* tree = malloc(sizeof *tree);
  tree->balance = 0;
  tree->data = data;
  tree->ref_count = 1;
  tree->sons[0] = left;
  tree->sons[1] = right;
  if (left) {
    left->ref_count++;
  }
  if (right) {
    right->ref_count++;
  }
  return tree;
}

/**
 * For the user: deletes the avl tree.
 * Internally: deletes it or not, it depend on the reference count.
 */
void imc_avl_unref (AVL* tree) {
  if (--tree->ref_count == 0) {
    imc_avl_unref(tree->sons[0]);
    imc_avl_unref(tree->sons[1]);
    if (tree->data) {
      unref_data(tree->data);
    }
    free(tree);
  }
}

/**
 * Deletes data. 
 */
void unref_data (DATA* data) {
  free(data);
}

/**
 * Copy a tree. Only the root is recreated and its refcount set to 1.
 * The children aren't copied, but their refcount is incremented of 1.
 */
AVL* clone_avl (AVL* old) {
  return imc_avl_create(old->data,old->sons[0],old->sons[1]);
}

/* ********************************
   Print / dump
*/

/**
 * Prints an avl to stdout.
 * Actually call internal_avl_dump to do the job.
 */
void imc_avl_dump (AVL* tree) {
  internal_avl_dump(stdout, tree, 0);
}

/**
 * Prints an avl to fp_out.
 * Each level of depth in the avl is printed with a larger indentation than
 * the previous one.
 */
void internal_avl_dump (FILE* fp_out, AVL* tree, unsigned int indent) {
  if (tree) {
    fprintf(fp_out, "%*s-%d\n", INDENT_SIZE*indent, "", tree->data->content);
    internal_avl_dump(fp_out, tree->sons[0], indent+1);
    internal_avl_dump(fp_out, tree->sons[1], indent+1);
  }
}


/* ********************************
   Insertion
*/

AVL* insert(AVL* avl, DATA* data) {
  int key = data->content;

  avl* new_avl = clone_avl(avl);
  avl* current = new_avl;
  
}
