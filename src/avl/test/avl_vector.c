
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "avl.h"
#include "avl_vector.h"

#define MAX(x,y) x < y ? y : x


/*******************
 *   Boxing & co   *
 *******************/
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
avl_data_t* clone_avl_data(avl_data_t* data) {
    avl_data_t* ret = malloc(sizeof *ret);
    ret->data = data->data;
    ret->index = data->index;
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

/*********************************
 * Vector manipulation functions *
 *********************************/
avl_vector_t* avl_vector_create() {
  avl_vector_t* ret = malloc(sizeof *ret);
  ret->vector = avl_make_empty_tree(_vector_compare);
  ret->max_index = -1;
  return ret;
}


vector_data_t* avl_vector_lookup(avl_vector_t* vec, int index) {
  avl_data_t* tmp = make_avl_data(NULL, index);
  avl_data_t* data = avl_search(vec->vector, tmp);
  free(tmp);
  if (data) {
    return data->data;
  } else {
    return NULL;
  }
}

avl_vector_t* avl_vector_push(avl_vector_t* vec, vector_data_t* data) {
  int index = vec->max_index + 1;
  avl_data_t* boxed_data = make_avl_data(data, index);

  avl_vector_t* new = malloc(sizeof *new);
  new->max_index = index;
  new->vector = avl_insert(vec->vector, boxed_data);

  return new;
}

int avl_vector_unref(avl_vector_t* vec) {
  if (vec) {
    avl_erase_tree(vec->vector);
    free(vec);
  }
  return 1;
}

void avl_vector_dump(avl_vector_t* vec) {
  printf("[ ");
  for (int i = 0; i <= vec->max_index; i++) {
    vector_data_t* data = avl_vector_lookup(vec, i);
    if (data) {
      char* data_string = data_as_string(data);
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
