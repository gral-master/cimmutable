#ifndef __RRB_VECTOR_H
#define __RRB_VECTOR_H

#include <vector.h>
#include <stdlib.h>

typedef struct _rrb {
    // Here some stuff.
} rrb_t;

int imc_rrb_size(imc_vector_t* vec);

imc_vector_t*  imc_rrb_update(imc_vector_t* vec, imc_data_t* data, int index);
imc_data_t* imc_rrb_lookup(imc_vector_t* vec, int index);

/* stack operations */

imc_vector_t* imc_rrb_push(imc_vector_t* vec, imc_data_t* data);
imc_vector_t* imc_rrb_pop (imc_vector_t* vec, imc_data_t* data);

int imc_rrb_split(imc_vector_t*  vec_in, int index,
                     imc_vector_t** vec_out1,
	                 imc_vector_t** vec_out2);

imc_vector_t* imc_rrb_merge(imc_vector_t* vec_front, imc_vector_t* vec_tail);

/* user-side memory management */

int imc_rrb_unref(imc_vector_t* vec);
void imc_rrb_dump(imc_vector_t* vec);

#endif
