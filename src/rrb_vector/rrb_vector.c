#include "rrb_vector.h"

imc_vector_t* imc_rrb_new() {
    imc_rrb_t* rrb = malloc(sizeof *rrb);
    rrb->update = imc_rrb_update;
    rrb->lookup = imc_rrb_lookup;
    rrb->push   = imc_rrb_push;
    rrb->pop    = imc_rrb_pop;
    rrb->merge  = imc_rrb_merge;
    rrb->split  = imc_rrb_split;
    rrb->size   = imc_rrb_size;
    rrb->unref  = imc_rrb_unref;
    rrb->dump   = imc_rrb_dump;
    rrb->entry  = NULL;
    return (imc_vector_t*) rrb;
}

int imc_rrb_size(imc_vector_t* vec) {
    imc_rrb_t* rrb = (imc_rrb_t*) vec;
    // Do some things.
    return 0;
}

imc_vector_t* imc_rrb_update(imc_vector_t* vec, imc_data_t* data, int index) {
    imc_rrb_t* rrb = (imc_rrb_t*) vec;
    // Do some things.
    return NULL;
}

imc_data_t* imc_rrb_lookup(imc_vector_t* vec, int index) {
    imc_rrb_t* rrb = (imc_rrb_t*) vec;
    // Do some things.
    return NULL;
}

imc_vector_t* imc_rrb_push(imc_vector_t* vec, imc_data_t* data) {
    imc_rrb_t* rrb = (imc_rrb_t*) vec;
    // Do some things.
    return NULL;
}

imc_vector_t* imc_rrb_pop (imc_vector_t* vec, imc_data_t* data) {
    imc_rrb_t* rrb = (imc_rrb_t*) vec;
    // Do some things.
    return NULL;
}

int imc_rrb_split(imc_vector_t*  vec_in, int index,
                  imc_vector_t** vec_out1,
	              imc_vector_t** vec_out2) {
    imc_rrb_t* rrb = (imc_rrb_t*) vec_in;
    // Do some things.
    return NULL;
}

imc_vector_t* imc_rrb_merge(imc_vector_t* vec_front, imc_vector_t* vec_tail) {
    imc_rrb_t* rrb_front = (imc_rrb_t*) vec_front;
    imc_rrb_t* rrb_tail  = (imc_rrb_t*) vec_tail;
    // Do some things.
    return NULL;
}

int imc_rrb_unref(imc_vector_t* vec) {
    imc_rrb_t* rrb = (imc_rrb_t*) vec;
    // Do some things.
    return 0;
}

void imc_rrb_dump(imc_vector_t* vec) {
    imc_rrb_t* rrb = (imc_rrb_t*) vec;
    // Print some stuff.
}
