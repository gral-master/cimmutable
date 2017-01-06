#pragma once

// #include <vector.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef int imc_data_t;

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

#define debug_print(fmt) \
    do { \
        if (DEBUG) { \
            fprintf(stderr, fmt); \
        } \
    } while (0)
#define debug_args(fmt, ...) \
    do { \
        if (DEBUG) { \
            fprintf(stderr, fmt, __VA_ARGS__); \
        } \
    } while (0)

rrb_vector_t *rrb_create();
rrb_vector_t *rrb_add(rrb_vector_t *rrb, imc_data_t *data);
