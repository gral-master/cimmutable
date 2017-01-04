#pragma once

#include <vector.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct _rrb {
  int level; // Depth of Node.
  int ref; // If 0, free it.
  int elements; // Number of elements contained.
  int *meta; // NULL if not needed.
  bool full; // Is it full ?
  bool leafs;
  union {
    struct _rrb **arr;
    imc_data_t **data;
  } children;
} rrb_vector_t;

rrb_vector_t *rrb_create();
rrb_vector_t *rrb_add(const rrb_vector_t *rrb, imc_data_t *data);
