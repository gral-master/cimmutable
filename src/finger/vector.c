#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fingers.h"

deep_t* finger_vector_create() {
    return make_empty_node();
}

int finger_vector_size(deep_t* vec) {
    return vector_size(vec);
}

deep_t* finger_vector_update(deep_t* vec,
                                int index,
                          finger_data_t* data) {
    return update_deep(vec, index, data);
}

finger_data_t* finger_vector_lookup(deep_t* vec,
                              int index) {
  return lookup(vec, index);
}

deep_t* finger_vector_push(deep_t* vec,
                              finger_data_t* data) {
    return append(vec, data, FINGER_RIGHT);
}

deep_t* finger_vector_pop(deep_t* vec,
                       finger_data_t** data) {
    return pop(vec, data);
}

int finger_vector_split(deep_t* vec_in,
                     int index,
                     deep_t** vec_out1,
                     deep_t** vec_out2) {
    return 0;
}

deep_t* finger_vector_merge(deep_t* vec_front,
                         deep_t* vec_tail) {
    return NULL;
}

int finger_vector_unref(deep_t* vec) {
    return unref_deep(vec);
}

void display(finger_data_t** data, int size) {
    for (int i = 0; i < size - 1; i++) {
        fprintf(stdout, "%d, ", *data[i]);
    }
    fprintf(stdout, "%d\n", *data[size - 1]);
}

void finger_vector_dump(deep_t* vec) {
    dump_deep(vec, 0, display);
}
