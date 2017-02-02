#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fingers.h"

deep_t* imc_vector_create() {
    return make_empty_node();
}

int imc_vector_size(deep_t* vec) {
    return 0;
}

deep_t* imc_vector_update(deep_t* vec,
                                int index,
                          finger_data_t* data) {
    return NULL;
}

finger_data_t* imc_vector_lookup(deep_t* vec,
                              int index) {
  return lookup(vec, index);
}

deep_t* imc_vector_push(deep_t* vec,
                              finger_data_t* data) {
    return append(vec, data, FINGER_RIGHT);
}

deep_t* imc_vector_pop(deep_t* vec,
                       finger_data_t** data) {
    return NULL;
}

int imc_vector_split(deep_t* vec_in,
                     int index,
                     deep_t** vec_out1,
                     deep_t** vec_out2) {
    return 0;
}

deep_t* imc_vector_merge(deep_t* vec_front,
                         deep_t* vec_tail) {
    return NULL;
}

int imc_vector_unref(deep_t* vec) {
    return unref_deep(vec);
}

void display(finger_data_t** data, int size) {
    for (int i = 0; i < size - 1; i++) {
        fprintf(stdout, "%d, ", *data[i]);
    }
    fprintf(stdout, "%d\n", *data[size - 1]);
}

void imc_vector_dump(deep_t* vec) {
    dump_deep(vec, 0, display);
}
