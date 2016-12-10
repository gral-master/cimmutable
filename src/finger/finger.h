#ifndef __FINGER_H
#define __FINGER_H

#include <vector.h>
#include <stdlib.h>

typedef struct _finger {
    // Here some stuff.
} finger_t;

int imc_finger_size(imc_vector_t* vec);

imc_vector_t*  imc_finger_update(imc_vector_t* vec, imc_data_t* data, int index);
imc_data_t* imc_finger_lookup(imc_vector_t* vec, int index);

/* stack operations */

imc_vector_t* imc_finger_push(imc_vector_t* vec, imc_data_t* data);
imc_vector_t* imc_finger_pop (imc_vector_t* vec, imc_data_t* data);

int imc_finger_split(imc_vector_t*  vec_in, int index,
                     imc_vector_t** vec_out1,
	                 imc_vector_t** vec_out2);

imc_vector_t* imc_finger_merge(imc_vector_t* vec_front, imc_vector_t* vec_tail);

/* user-side memory management */

int imc_finger_unref(imc_vector_t* vec);
void imc_finger_dump(imc_vector_t* vec);

#endif
