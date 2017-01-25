#ifndef _AVL_VECTOR
#define _AVL_VECTOR

#include "avl.h"

typedef struct _avl_vector_t {
  avl_tree* vector;
  int max_index;
  char* (*data_as_string)(void* data);
} avl_vector_t;

avl_vector_t* avl_vector_create(char* (*data_as_string)(void* data));

int avl_vector_size (avl_vector_t* vec);

avl_vector_t* avl_vector_update(avl_vector_t* vec, int index,
				void* data);

void* avl_vector_lookup(avl_vector_t* vec, int index);

avl_vector_t* avl_vector_push(avl_vector_t* vec, void* data);

avl_vector_t* avl_vector_pop(avl_vector_t* vec, void** data);

avl_vector_t* avl_vector_merge(avl_vector_t* vec_front, avl_vector_t* vec_tail);

int avl_vector_split(avl_vector_t* vec_in, int index,
		     avl_vector_t** vec_out1, avl_vector_t** vec_out2);

int avl_vector_unref(avl_vector_t* vec);

void avl_vector_dump_ignore_empty(avl_vector_t* vec);
void avl_vector_dump(avl_vector_t* vec);



#endif
