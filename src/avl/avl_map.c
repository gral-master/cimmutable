
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "avl.h"
#include "avl_map.h"

#define MAX(x,y) x < y ? y : x

/*******************
 *   Boxing & co   *
 *******************/
typedef struct _avl_map_data {
  void*  key;
  void* data;
} _avl_map_data_t;
_avl_map_data_t* make_avl_data(void* key, void* data) {
  _avl_map_data_t* ret = malloc(sizeof *ret);
  ret->data = data;
  ret->key = key;
  return ret;
}
_avl_map_data_t* clone_avl_data(_avl_map_data_t* data) {
    _avl_map_data_t* ret = malloc(sizeof *ret);
    ret->data = data->data;
    ret->key = data->key;
    return ret;
}

/*********************************
 *   Map manipulation functions  *
 *********************************/
avl_map_t* avl_map_create(char* (*key_as_string)(void*),
			  char* (*data_as_string)(void*),
			  int (*key_compare)(void*,void*)) {
  avl_map_t* ret = malloc(sizeof *ret);
  ret->map = avl_make_empty_tree(key_compare);
  ret->key_as_string  = key_as_string;
  ret->data_as_string = data_as_string;
  return ret;
}

int avl_map_size (avl_map_t* map) {
  return map->map->size;
}

avl_map_t* avl_map_update(avl_map_t* map, void* key, void* data) {
  avl_map_t* new = malloc(sizeof *new);
  _avl_map_data_t* boxed_data = make_avl_data(key, data);

  new->map = avl_insert(map->map, boxed_data);
  new->key_as_string  = map->key_as_string;
  new->data_as_string = map->data_as_string;
  
  return new;
}

void* avl_map_lookup(avl_map_t* map, void* key) {
  _avl_map_data_t* tmp = make_avl_data(key, NULL);
  _avl_map_data_t* data = avl_search(map->map, tmp);
  free(tmp);
  if (data) {
    return data->data;
  } else {
    return NULL;
  }
}

avl_map_t* avl_map_remove(avl_map_t* map, void* key,
			  void** data) {
  _avl_map_data_t* tmp = make_avl_data(key, NULL);
  void* return_data = NULL;

  avl_map_t* new = malloc(sizeof *new);

  new->key_as_string  = map->key_as_string;
  new->data_as_string = map->data_as_string;
  new->map = avl_remove(map->map, tmp, &return_data);
  free(tmp);

  if (return_data) {
    *data = ((_avl_map_data_t*)return_data)->data;
  } else {
    *data = NULL;
  }
  return new;
}

void _map_keys_aux(avl_node* node, void** keys, int* index) {
  if (node) {
    keys[(*index)++] = ((_avl_map_data_t*)node->data)->key;
    _map_keys_aux(node->sons[0], keys, index);
    _map_keys_aux(node->sons[1], keys, index);
  }
}

void** avl_map_keys(avl_map_t* map) {
  void** keys = malloc(map->map->size * sizeof(*keys));

  int index = 0;
  _map_keys_aux(map->map->root, keys, &index);

  return keys;
}


map_iterator_t* avl_map_create_iterator(avl_map_t* map) {
  map_iterator_t* iterator = malloc(sizeof *iterator);
  iterator->current = map->map->root;
  iterator->next    = NULL;

  return iterator;
}


int avl_map_iterator_next(map_iterator_t** iterator,
			  void** key, void** data) {
  map_iterator_t* deref_iter = *iterator;
  if (deref_iter) {
    avl_node* current = deref_iter->current;
    *key  = ((_avl_map_data_t*)current->data)->key;
    *data = ((_avl_map_data_t*)current->data)->data;
    map_iterator_t* next = deref_iter->next;
    for (int i = 0; i <= 1; i++) {
      if (current->sons[i]) {
	map_iterator_t* tmp = malloc(sizeof *tmp);
	tmp->current = current->sons[i];
	tmp->next = next;
	next = tmp;
      }
    }
    free(deref_iter);
    *iterator = next;
    return 1;
  } else {
    return 0;
  }
}


int avl_map_unref(avl_map_t* map) {
  if (map) {
    avl_erase_tree(map->map);
    free(map);
  }
  return 1;
}

int _max_key_size(avl_node* node, char* (*key_as_string)(void*)) {
  if (node) {
    int n = strlen((*key_as_string)(((_avl_map_data_t*)node->data)->key));
    for (int i = 0; i <= 1; i++) {
      if (node->sons[i]) {
	int tmp = _max_key_size(node->sons[i], key_as_string);
	n = MAX(n,tmp);
      }
    }
    return n;
  } else {
    return 0;
  }  
}

void _map_dump_aux(avl_node* node, int padding,
		   char* (*key_as_string)(void*),
		   char* (*data_as_string)(void*)) {
  if (node) {
    printf("\n\t%-*s => %s,", padding,
	   (*key_as_string)(((_avl_map_data_t*)node->data)->key),
	   (*data_as_string)(((_avl_map_data_t*)node->data)->data));
    _map_dump_aux(node->sons[0], padding, key_as_string, data_as_string);
    _map_dump_aux(node->sons[1], padding, key_as_string, data_as_string);
  }
}

void avl_map_dump(avl_map_t* map) {
  printf("{ ");
  int padding = _max_key_size(map->map->root, map->key_as_string);
  _map_dump_aux(map->map->root, padding,
		map->key_as_string, map->data_as_string);
  printf("\b \n}\n");
}
