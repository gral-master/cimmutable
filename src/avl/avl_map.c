
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
struct _avl_data_t {
  map_key_t*  key;
  map_data_t* data;
};
avl_data_t* make_avl_data(map_key_t* key, map_data_t* data) {
  avl_data_t* ret = malloc(sizeof *ret);
  ret->data = data;
  ret->key = key;
  return ret;
}
avl_data_t* clone_avl_data(avl_data_t* data) {
    avl_data_t* ret = malloc(sizeof *ret);
    ret->data = data->data;
    ret->key = data->key;
    return ret;
}
char* avl_data_as_string_key(avl_data_t* data) {
  return key_as_string(data->key);
}
char* avl_data_as_string_data(avl_data_t* data) {
  return data_as_string(data->data);
}
int _map_compare (avl_data_t* d1, avl_data_t* d2) {
  return key_compare(d1->key, d2->key);
}
char* _avl_data_as_string (avl_data_t* data) {
  char* key_string = key_as_string(data->key);
  char* data_string = data_as_string(data->data);
  int size = strlen(key_string) + strlen(data_string) + 4 + 1;
  char* buf = malloc(size * sizeof(char));
  sprintf(buf, "%s => %s", key_string, data_string);
  return buf;
}

/*********************************
 *   Map manipulation functions  *
 *********************************/
avl_map_t* avl_map_create() {
  avl_map_t* ret = malloc(sizeof *ret);
  ret->map = avl_make_empty_tree(_map_compare);
  return ret;
}

int avl_map_size (avl_map_t* map) {
  return map->map->size;
}

avl_map_t* avl_map_update(avl_map_t* map, map_key_t* key,
			  map_data_t* data) {
  avl_map_t* new = malloc(sizeof *new);
  avl_data_t* boxed_data = make_avl_data(key, data);

  new->map = avl_insert(map->map, boxed_data);
  
  return new;
}

map_data_t* avl_map_lookup(avl_map_t* map, map_key_t* key) {
  avl_data_t* tmp = make_avl_data(key, NULL);
  avl_data_t* data = avl_search(map->map, tmp);
  free(tmp);
  if (data) {
    return data->data;
  } else {
    return NULL;
  }
}

avl_map_t* avl_map_remove(avl_map_t* map, map_key_t* key,
			  map_data_t** data) {
  avl_data_t* tmp = make_avl_data(key, NULL);
  avl_data_t* return_data = NULL;

  avl_map_t* new = malloc(sizeof *new);
  
  new->map = avl_remove(map->map, tmp, &return_data);
  free(tmp);

  if (return_data) {
    *data = return_data->data;
  } else {
    *data = NULL;
  }
  return new;
}

void _map_keys_aux(avl_node* node, map_key_t** keys, int* index) {
  if (node) {
    keys[(*index)++] = node->data->key;
    _map_keys_aux(node->sons[0], keys, index);
    _map_keys_aux(node->sons[1], keys, index);
  }
}

map_key_t** avl_map_keys(avl_map_t* map) {
  map_key_t** keys = malloc(map->map->size * sizeof(*keys));

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
			  map_key_t** key, map_data_t** data) {
  map_iterator_t* deref_iter = *iterator;
  if (deref_iter) {
    avl_node* current = deref_iter->current;
    *key  = current->data->key;
    *data = current->data->data;
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

int _max_key_size(avl_node* node) {
  if (node) {
    int n = strlen(key_as_string(node->data->key));
    for (int i = 0; i <= 1; i++) {
      if (node->sons[i]) {
	int tmp = _max_key_size(node->sons[i]);
	n = MAX(n,tmp);
      }
    }
    return n;
  } else {
    return 0;
  }  
}

void _map_dump_aux(avl_node* node, int padding) {
  if (node) {
    printf("\n\t%-*s => %s,", padding, key_as_string(node->data->key),
	   data_as_string(node->data->data));
    _map_dump_aux(node->sons[0], padding);
    _map_dump_aux(node->sons[1], padding);
  }
}

void avl_map_dump(avl_map_t* map) {
  printf("{ ");
  int padding = _max_key_size(map->map->root);
  _map_dump_aux(map->map->root, padding);
  printf("\b \n}\n");
}
