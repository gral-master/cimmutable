
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "avl.h"
#include "avl_vector.h"

struct _avl_vector_t {
  avl_tree* vector;
  int max_index;
};

struct _avl_data_t {
  int index;
  vector_data_t* data;
};

avl_data_t* make_avl_data(vector_data_t* data, int index) {
  avl_data_t* ret = malloc(sizeof *ret);
  ret->data = data;
  ret->index = index;
  return ret;
}

char* avl_data_as_string(avl_data_t* data) {
  return data_as_string(data->data);
}


int _vector_compare (avl_data_t* d1, avl_data_t* d2) {
  if (d1->index == d2->index) return 0;
  if (d1->index < d2->index) return -1;
  return 1;
}


avl_vector_t* avl_vector_create() {
  avl_vector_t* ret = malloc(sizeof *ret);
  ret->vector = avl_make_empty_tree(_vector_compare);
  ret->max_index = 0;
  return ret;
}


int avl_vector_size (avl_vector_t* vec) {
  if (vec) return vec->max_index;
  else return -1 ;
}


avl_vector_t* avl_vector_update(avl_vector_t* vec, int index,
				vector_data_t* data) {
  avl_vector_t* new = malloc(sizeof *new);
  if (index > vec->max_index) {
    new->max_index = index;
  } else {
    new->max_index = vec->max_index;
  }

  avl_data_t* boxed_data = make_avl_data(data, index);

  new->vector = avl_insert(vec->vector, boxed_data);
  
  return new;
}

vector_data_t* lookup(avl_vector_t* vec, int index) {
  avl_data_t* tmp = make_avl_data(NULL, index);
  avl_data_t* data = avl_search(vec->vector, tmp);
  free(tmp);
  return data->data;
}




void avl_vector_dump(avl_vector_t* vec) {
  avl_traverse_and_print(vec->vector);
}
