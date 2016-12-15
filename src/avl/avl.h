#ifndef _AVL_H
#define _AVL_H


typedef struct _imc_data_t imc_data_t;
typedef struct _imc_avl_tree_t imc_avl_tree_t;


imc_avl_tree_t* imc_avl_create ();

void imc_avl_unref (imc_avl_tree_t* tree);

void imc_avl_dump (imc_avl_tree_t* tree);





#endif
