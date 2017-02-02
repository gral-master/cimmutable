#ifndef _VECTOR_H
#define _VECTOR_H


deep_t* imc_vector_create();

int imc_vector_size(deep_t* vec);

deep_t* imc_vector_update(deep_t* vec,
				int index,
				finger_data_t* data);

finger_data_t* imc_vector_lookup(deep_t* vec,
			      int index);

/* stack operations */

// add at the end <--- DOXYGENIZE PLEASE!
deep_t* imc_vector_push(deep_t* vec,
			      finger_data_t* data);

deep_t* imc_vector_pop(deep_t* vec,
			     finger_data_t** data);

int imc_vector_split(deep_t* vec_in,
		     int index,
		     deep_t** vec_out1,
		     deep_t** vec_out2);

deep_t* imc_vector_merge(deep_t* vec_front,
			       deep_t* vec_tail);

/* user-side memory management */

int imc_vector_unref(deep_t* vec);

void imc_vector_dump(deep_t* vec);

#endif
