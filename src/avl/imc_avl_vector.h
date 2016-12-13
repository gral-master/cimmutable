#ifndef IMC_AVL_VECTOR
#define IMC_AVL_VECTOR

#include "imc_avl.h"

typedef imc_data_t;
typedef imc_key_t;
//----------------------------------------------------------------------------//
//----------------------------Data Structure----------------------------------//
//----------------------------------------------------------------------------//
typedef struct imc_avl_vector_t{
    imc_avl_node_t* tree;
    int size;
} imc_avl_vector_t;


//----------------------------------------------------------------------------//
//--------------------------Vector functions----------------------------------//
//----------------------------------------------------------------------------//
imc_avl_vector_t* imc_avl_vector_create();

int imc_avl_vector_size(imc_avl_vector_t* vec);

imc_avl_vector_t* imc_avl_vector_update(imc_avl_vector_t* vec, int index, imc_data_t* data);

imc_data_t* imc_avl_vector_lookup(imc_avl_vector_t* vec, int index);

/* stack operations */

// add at the end <--- DOXYGENIZE PLEASE!
imc_avl_vector_t* imc_avl_vector_push(imc_avl_vector_t* vec,
			      imc_data_t* data);

imc_avl_vector_t* imc_avl_vector_pop(imc_avl_vector_t* vec,
			     imc_data_t** data);

int imc_avl_vector_split(imc_avl_vector_t* vec_in,
		     int index,
		     imc_avl_vector_t** vec_out1,
		     imc_avl_vector_t** vec_out2);

imc_avl_vector_t* imc_avl_vector_merge(imc_avl_vector_t* vec_front,
			       imc_avl_vector_t* vec_tail);

/* user-side memory management */

int imc_avl_vector_unref(imc_avl_vector_t* vec);

void imc_avl_vector_dump(imc_avl_vector_t* vec);
#endif
