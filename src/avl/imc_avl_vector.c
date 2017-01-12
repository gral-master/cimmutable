#include "imc_avl_map.h"
#include <stdlib.h>

imc_vector_avl_t* imc_vector_avl_create(){
    imc_vector_avl_t * vect = malloc(sizeof(imc_vector_avl_t));
    vect->tree = NULL;
    vect->size = 0;
    return vect;
}

int imc_vector_avl_size(imc_vector_avl_t* vec){
    return vec->size;
}

imc_vector_avl_t* imc_vector_avl_update(imc_vector_avl_t* vec, int index,
                                        imc_data_t* data){

}

imc_data_t* imc_vector_avl_lookup(imc_vector_avl_t* vec, int index);

/* stack operations */

// add at the end <--- DOXYGENIZE PLEASE!
imc_vector_avl_t* imc_vector_avl_push(imc_vector_avl_t* vec,
			      imc_data_t* data){

}

imc_vector_avl_t* imc_vector_avl_pop(imc_vector_avl_t* vec,
			     imc_data_t** data){

}

int imc_vector_avl_split(imc_vector_avl_t* vec_in,
		     int index,
		     imc_vector_avl_t** vec_out1,
		     imc_vector_avl_t** vec_out2){

}

imc_vector_avl_t* imc_vector_avl_merge(imc_vector_avl_t* vec_front,
			       imc_vector_avl_t* vec_tail){

}

/* user-side memory management */

int imc_vector_avl_unref(imc_vector_avl_t* vec){
    imc_avl_unref(vec->tree);
    free(vec);
    return 1;
}

void imc_vector_avl_dump(imc_vector_avl_t* vec){

}
