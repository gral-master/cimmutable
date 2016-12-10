#include "avl.h"

imc_vector_t* imc_avl_new() {
    imc_avl_t* avl = malloc(sizeof *avl);
    avl->update = imc_avl_update;
    avl->lookup = imc_avl_lookup;
    avl->push   = imc_avl_push;
    avl->pop    = imc_avl_pop;
    avl->merge  = imc_avl_merge;
    avl->split  = imc_avl_split;
    avl->size   = imc_avl_size;
    avl->unref  = imc_avl_unref;
    avl->dump   = imc_avl_dump;
    avl->entry  = NULL;
    return (imc_vector_t*) avl;
}

int imc_avl_size(imc_vector_t* vec) {
    imc_avl_t* avl = (imc_avl_t*) vec;
    // Do some things.
    return 0;
}

imc_vector_t* imc_avl_update(imc_vector_t* vec, imc_data_t* data, int index) {
    imc_avl_t* avl = (imc_avl_t*) vec;
    // Do some things.
    return NULL;
}

imc_data_t* imc_avl_lookup(imc_vector_t* vec, int index) {
    imc_avl_t* avl = (imc_avl_t*) vec;
    // Do some things.
    return NULL;
}

imc_vector_t* imc_avl_push(imc_vector_t* vec, imc_data_t* data) {
    imc_avl_t* avl = (imc_avl_t*) vec;
    // Do some things.
    return NULL;
}

imc_vector_t* imc_avl_pop (imc_vector_t* vec, imc_data_t* data) {
    imc_avl_t* avl = (imc_avl_t*) vec;
    // Do some things.
    return NULL;
}

int imc_avl_split(imc_vector_t*  vec_in, int index,
                  imc_vector_t** vec_out1,
	              imc_vector_t** vec_out2) {
    imc_avl_t* avl = (imc_avl_t*) vec_in;
    // Do some things.
    return NULL;
}

imc_vector_t* imc_avl_merge(imc_vector_t* vec_front, imc_vector_t* vec_tail) {
    imc_avl_t* avl_front = (imc_avl_t*) vec_front;
    imc_avl_t* avl_tail  = (imc_avl_t*) vec_tail;
    // Do some things.
    return NULL;
}

int imc_avl_unref(imc_vector_t* vec) {
    imc_avl_t* avl = (imc_avl_t*) vec;
    // Do some things.
    return 0;
}

void imc_avl_dump(imc_vector_t* vec) {
    imc_avl_t* avl = (imc_avl_t*) vec;
    // Print some stuff.
}
