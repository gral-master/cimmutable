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

int avl_vector_size (avl_vector_t* vec);

avl_vector_t* avl_vector_update(avl_vector_t* vec, int index,
				vector_data_t* data);

vector_data_t* avl_vector_lookup(avl_vector_t* vec, int index);

avl_vector_t* avl_vector_push(avl_vector_t* vec, vector_data_t* data);

avl_vector_t* avl_vector_pop(avl_vector_t* vec, vector_data_t** data);

avl_vector_t* avl_vector_merge(avl_vector_t* vec_front, avl_vector_t* vec_tail);

int avl_vector_split(avl_vector_t* vec_in, int index,
		     avl_vector_t** vec_out1, avl_vector_t** vec_out2);

int avl_vector_unref(avl_vector_t* vec);

void avl_vector_dump_ignore_empty(avl_vector_t* vec);
void avl_vector_dump(avl_vector_t* vec);



#endif
