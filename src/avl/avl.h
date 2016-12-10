#ifndef __AVL_H
#define __AVL_H

#include <vector.h>
#include <stdlib.h>

typedef struct _avl {
    // Here some stuff.
} avl_t;

int imc_avl_size(imc_vector_t* vec);

imc_vector_t*  imc_avl_update(imc_vector_t* vec, imc_data_t* data, int index);
imc_data_t* imc_avl_lookup(imc_vector_t* vec, int index);

/* stack operations */

imc_vector_t* imc_avl_push(imc_vector_t* vec, imc_data_t* data);
imc_vector_t* imc_avl_pop (imc_vector_t* vec, imc_data_t* data);

int imc_avl_split(imc_vector_t*  vec_in, int index,
                  imc_vector_t** vec_out1,
	              imc_vector_t** vec_out2);

imc_vector_t* imc_avl_merge(imc_vector_t* vec_front, imc_vector_t* vec_tail);

/* user-side memory management */

int imc_avl_unref(imc_vector_t* vec);
void imc_avl_dump(imc_vector_t* vec);

#endif
