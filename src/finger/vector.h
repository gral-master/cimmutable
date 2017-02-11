#ifndef _VECTOR_H
#define _VECTOR_H

#include "fingers.h"

deep_t* finger_vector_create();

int finger_vector_size(deep_t* vec);

deep_t* finger_vector_update(deep_t* vec, int index, finger_data_t* data);

finger_data_t* finger_vector_lookup(deep_t* vec, int index);

/* stack operations */

deep_t* finger_vector_push(deep_t* vec, finger_data_t* data);

deep_t* finger_vector_pop(deep_t* vec, finger_data_t** data);

int finger_vector_split(deep_t* vec_in, int index, deep_t** vec_out1, deep_t** vec_out2);

deep_t* finger_vector_merge(deep_t* vec_front, deep_t* vec_tail);

/* user-side memory management */

int finger_vector_unref(deep_t* vec);

void finger_vector_dump(deep_t* vec);

#endif
