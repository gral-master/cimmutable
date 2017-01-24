
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


int avl_vector_size (avl_vector_t* vec) {
  return vec->max_index + 1; /* +1 because the array is 0-indexed */
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

avl_vector_t* avl_vector_pop(avl_vector_t* vec, vector_data_t** data) {
  int index = vec->max_index;
  if (index >= 0) {
    avl_data_t* tmp = make_avl_data(NULL, index);

    avl_vector_t* new = malloc(sizeof *new);
    new->max_index = index - 1;

    avl_data_t* return_data = NULL;

    new->vector = avl_remove(vec->vector, tmp, &return_data);
    free(tmp);
  
    if (return_data) {
      *data = return_data->data;
      free(return_data);
    } else {
      *data = NULL;
    }
    return new;
  } else { /* empty vector */
    avl_vector_t* new = avl_vector_create();
    *data = NULL;
    return new;
  }
}


void _merge_vector_internal(avl_tree* ret, avl_node* orig, int shift) {
  if (orig != NULL) {
    avl_data_t* data = make_avl_data(orig->data->data,
				     shift + orig->data->index);
    
    avl_insert_mutable(ret, data);
    _merge_vector_internal(ret, orig->sons[0], shift);
    _merge_vector_internal(ret, orig->sons[1], shift);
  }
}


avl_vector_t* avl_vector_merge (avl_vector_t* vec_front,
				avl_vector_t* vec_tail) {
  avl_vector_t* new = avl_vector_create();
  _merge_vector_internal(new->vector, vec_front->vector->root, 0);
  _merge_vector_internal(new->vector, vec_tail->vector->root,
			 vec_front->max_index+1);
  new->max_index = vec_front->max_index + vec_tail->max_index + 1;
  return new;
}


void _split_vector_internal(avl_node* node, int index,
			    avl_vector_t* vec_out1, avl_vector_t* vec_out2) {
  if (node) {
    avl_data_t* data = clone_avl_data(node->data);
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
int avl_vector_split(avl_vector_t* vec_in, int index,
		     avl_vector_t** vec_out1, avl_vector_t** vec_out2) {
  *vec_out1 = avl_vector_create();
  *vec_out2 = avl_vector_create();
  if (vec_in->max_index >= 0) {
    _split_vector_internal(vec_in->vector->root, index, *vec_out1, *vec_out2);
    return 1;
  } else {
    return 0;
  }
}

int avl_vector_unref(avl_vector_t* vec) {
  if (vec) {
    avl_erase_tree(vec->vector);
    free(vec);
  }
  return 1;
}

void avl_vector_dump_ignore_empty(avl_vector_t* vec) {
  avl_traverse_and_print(vec->vector);
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
