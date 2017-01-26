
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "avl.h"
#include "avl_vector.h"

#define MAX(x,y) x < y ? y : x

struct _avl_vector_t {
  avl_tree* vector;
  int max_index;
  char* (*data_as_string)(void* data);
};

/*******************
 *   Boxing & co   *
 *******************/
typedef struct _avl_vector_data {
  void* data;
  int index;
} _avl_vector_data_t;

_avl_vector_data_t* make_avl_data(void* data, int index) {
  _avl_vector_data_t* ret = malloc(sizeof *ret);
  ret->data = data;
  ret->index = index;
  return ret;
}
_avl_vector_data_t* clone_avl_data(_avl_vector_data_t* data) {
    _avl_vector_data_t* ret = malloc(sizeof *ret);
    ret->data = data->data;
    ret->index = data->index;
    return ret;
}
int _vector_compare (void* d1, void* d2) {
  if (((_avl_vector_data_t*)d1)->index == (((_avl_vector_data_t*)d2)->index)) return 0;
  if (((_avl_vector_data_t*)d1)->index < (((_avl_vector_data_t*)d2)->index)) return -1;
  return 1;
}

/************************
 *   User side boxing   *
 ************************/
#ifndef _INT_BOX_C
#define _INT_BOX_C
/* int box */
int_box_t* make_int_box(int i) {
  int_box_t* box = malloc(sizeof(*box));
  *box = i;
  return box;
}
char* int_box_as_string(void* data) {
  char* buf = malloc(20 * sizeof(char)); /* 20 char is enough to hold 2**64. */
  sprintf(buf, "%d", *((int_box_t*)data));
  return buf;
}
int compare_int_keys(void* key1, void* key2) {
  if (*((int_box_t*)key1) == *((int_box_t*)key2)) return 0;
  else if (*((int_box_t*)key1) < *((int_box_t*)key2)) return -1;
  return 1;
}
#endif

#ifndef _STRING_BOX_C
#define _STRING_BOX_C
/* char* box */
string_box_t* make_string_box(char* str) {
  string_box_t* ret = malloc(sizeof *ret);
  *ret = str;
  return ret;
}
char* string_box_as_string(void* box) {
  return strdup(*((string_box_t*)box));
}
int compare_string_keys(void* key1, void* key2) {
  return strcmp(*((string_box_t*)key1), *((string_box_t*)key2));
}
#endif

/*********************************
 * Vector manipulation functions *
 *********************************/
avl_vector_t* avl_vector_create(char* (*data_as_string)(void* data)) {
  avl_vector_t* ret = malloc(sizeof *ret);
  ret->vector = avl_make_empty_tree(_vector_compare);
  ret->max_index = -1;
  ret->data_as_string = data_as_string;
  return ret;
}


int avl_vector_size (const avl_vector_t* vec) {
  return vec->max_index + 1; /* +1 because the array is 0-indexed */
}


avl_vector_t* avl_vector_update(const avl_vector_t* vec, int index,
				void* data) {
  avl_vector_t* new = malloc(sizeof *new);
  if (index > vec->max_index) {
    new->max_index = index;
  } else {
    new->max_index = vec->max_index;
  }

  _avl_vector_data_t* boxed_data = make_avl_data(data, index);

  new->vector = avl_insert(vec->vector, boxed_data);
  new->data_as_string = vec->data_as_string;
  
  return new;
}

void* avl_vector_lookup(const avl_vector_t* vec, int index) {
  _avl_vector_data_t* tmp = make_avl_data(NULL, index);
  _avl_vector_data_t* data = avl_search(vec->vector, tmp);
  free(tmp);
  if (data) {
    return data->data;
  } else {
    return NULL;
  }
}

avl_vector_t* avl_vector_push(const avl_vector_t* vec, void* data) {
  int index = vec->max_index + 1;
  _avl_vector_data_t* boxed_data = make_avl_data(data, index);

  avl_vector_t* new = malloc(sizeof *new);
  new->max_index = index;
  new->vector = avl_insert(vec->vector, boxed_data);
  new->data_as_string = vec->data_as_string;

  return new;
}

avl_vector_t* avl_vector_pop(const avl_vector_t* vec, void** data) {
  int index = vec->max_index;
  if (index >= 0) {
    _avl_vector_data_t* tmp = make_avl_data(NULL, index);

    avl_vector_t* new = malloc(sizeof *new);
    new->max_index = index - 1;
    new->data_as_string = vec->data_as_string;

    void* return_data = NULL;

    new->vector = avl_remove(vec->vector, tmp, &return_data);
    free(tmp);
  
    if (return_data) {
      *data = ((_avl_vector_data_t*)return_data)->data;
      free(return_data);
    } else {
      *data = NULL;
    }
    return new;
  } else { /* empty vector */
    avl_vector_t* new = avl_vector_create(vec->data_as_string);
    *data = NULL;
    return new;
  }
}


void _merge_vector_internal(avl_tree* ret, avl_node* orig, int shift) {
  if (orig != NULL) {
    _avl_vector_data_t* data =
      make_avl_data(((_avl_vector_data_t*)orig->data)->data,
		    shift + ((_avl_vector_data_t*)orig->data)->index);
    
    avl_insert_mutable(ret, data);
    _merge_vector_internal(ret, orig->sons[0], shift);
    _merge_vector_internal(ret, orig->sons[1], shift);
  }
}


avl_vector_t* avl_vector_merge (const avl_vector_t* vec_front,
				const avl_vector_t* vec_tail) {
  avl_vector_t* new = avl_vector_create(vec_front->data_as_string);
  new->data_as_string = vec_front->data_as_string;
  _merge_vector_internal(new->vector, vec_front->vector->root, 0);
  _merge_vector_internal(new->vector, vec_tail->vector->root,
			 vec_front->max_index+1);
  new->max_index = vec_front->max_index + vec_tail->max_index + 1;
  return new;
}


void _split_vector_internal(avl_node* node, int index,
			    avl_vector_t* vec_out1, avl_vector_t* vec_out2) {
  if (node) {
    _avl_vector_data_t* data = clone_avl_data(node->data);
    if (data->index <= index) {
      avl_insert_mutable(vec_out1->vector, data);
      vec_out1->max_index = MAX(vec_out1->max_index, data->index);
    } else {
      data->index -= index+1;
      avl_insert_mutable(vec_out2->vector, data);
      vec_out2->max_index = MAX(vec_out2->max_index, data->index);
    }
    _split_vector_internal(node->sons[0], index, vec_out1, vec_out2);
    _split_vector_internal(node->sons[1], index, vec_out1, vec_out2);
  }
}

/* split ==> [0..index] [index+1..end] */
int avl_vector_split(const avl_vector_t* vec_in, int index,
		     avl_vector_t** vec_out1, avl_vector_t** vec_out2) {
  *vec_out1 = avl_vector_create(vec_in->data_as_string);
  *vec_out2 = avl_vector_create(vec_in->data_as_string);
  if (vec_in->max_index >= 0) {
    _split_vector_internal(vec_in->vector->root, index, *vec_out1, *vec_out2);
    return 1;
  } else {
    return 0;
  }
}

void avl_vector_unref(avl_vector_t* vec) {
  if (vec) {
    avl_erase_tree(vec->vector);
    free(vec);
  }
}

void avl_vector_dump(const avl_vector_t* vec) {
  printf("[ ");
  for (int i = 0; i <= vec->max_index; i++) {
    void* data = avl_vector_lookup(vec, i);
    if (data) {
      char* data_string = (*vec->data_as_string)(data);
      printf("%s, ", data_string);
      free(data_string);
    } else {
      printf("_, ");
    }
  }
  if (vec->max_index >= 0) {
    printf("\b\b ");
  }
  printf("]\n");
}
