#ifndef _AVL_VECTOR
#define _AVL_VECTOR

#include "avl.h"

typedef struct _avl_vector_t {
  avl_tree* vector;
  int max_index;
} avl_vector_t;

typedef struct _vector_data_t vector_data_t;
char* data_as_string(vector_data_t* data);



avl_vector_t* avl_vector_create();

avl_vector_t* avl_vector_push(avl_vector_t* vec, vector_data_t* data);

int avl_vector_unref(avl_vector_t* vec);

void avl_vector_dump(avl_vector_t* vec);



#endif
