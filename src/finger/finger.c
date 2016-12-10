#include "finger.h"

imc_vector_t* imc_finger_new() {
    imc_finger_t* finger = malloc(sizeof *finger);
    finger->update = imc_finger_update;
    finger->lookup = imc_finger_lookup;
    finger->push   = imc_finger_push;
    finger->pop    = imc_finger_pop;
    finger->merge  = imc_finger_merge;
    finger->split  = imc_finger_split;
    finger->size   = imc_finger_size;
    finger->unref  = imc_finger_unref;
    finger->dump   = imc_finger_dump;
    finger->entry  = NULL;
    return (imc_vector_t*) finger;
}

int imc_finger_size(imc_vector_t* vec) {
    imc_finger_t* finger = (imc_finger_t*) vec;
    // Do some things.
    return 0;
}

imc_vector_t* imc_finger_update(imc_vector_t* vec, imc_data_t* data, int index) {
    imc_finger_t* finger = (imc_finger_t*) vec;
    // Do some things.
    return NULL;
}

imc_data_t* imc_finger_lookup(imc_vector_t* vec, int index) {
    imc_finger_t* finger = (imc_finger_t*) vec;
    // Do some things.
    return NULL;
}

imc_vector_t* imc_finger_push(imc_vector_t* vec, imc_data_t* data) {
    imc_finger_t* finger = (imc_finger_t*) vec;
    // Do some things.
    return NULL;
}

imc_vector_t* imc_finger_pop (imc_vector_t* vec, imc_data_t* data) {
    imc_finger_t* finger = (imc_finger_t*) vec;
    // Do some things.
    return NULL;
}

int imc_finger_split(imc_vector_t*  vec_in, int index,
                  imc_vector_t** vec_out1,
	              imc_vector_t** vec_out2) {
    imc_finger_t* finger = (imc_finger_t*) vec_in;
    // Do some things.
    return NULL;
}

imc_vector_t* imc_finger_merge(imc_vector_t* vec_front, imc_vector_t* vec_tail) {
    imc_finger_t* finger_front = (imc_finger_t*) vec_front;
    imc_finger_t* finger_tail  = (imc_finger_t*) vec_tail;
    // Do some things.
    return NULL;
}

int imc_finger_unref(imc_vector_t* vec) {
    imc_finger_t* finger = (imc_finger_t*) vec;
    // Do some things.
    return 0;
}

void imc_finger_dump(imc_vector_t* vec) {
    imc_finger_t* finger = (imc_finger_t*) vec;
    // Print some stuff.
}
