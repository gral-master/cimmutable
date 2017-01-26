
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "avl.h"
#include "avl_map.h"

#define MAX(x,y) x < y ? y : x
struct _avl_map_t{
  avl_tree* map;
  char* (*key_as_string)(void*);
  char* (*data_as_string)(void*);
};
struct _map_iterator_t{
  avl_node* current;
  struct _map_iterator_t* next;
};

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

int avl_map_size (const avl_map_t* map) {
  return map->map->size;
}

avl_map_t* avl_map_update(const avl_map_t* map, void* key, void* data) {
  avl_map_t* new = malloc(sizeof *new);
  _avl_map_data_t* boxed_data = make_avl_data(key, data);

  new->map = avl_insert(map->map, boxed_data);
  new->key_as_string  = map->key_as_string;
  new->data_as_string = map->data_as_string;
  
  return new;
}

void* avl_map_lookup(const avl_map_t* map, void* key) {
  _avl_map_data_t* tmp = make_avl_data(key, NULL);
  _avl_map_data_t* data = avl_search(map->map, tmp);
  free(tmp);
  if (data) {
    return data->data;
  } else {
    return NULL;
  }
}

avl_map_t* avl_map_remove(const avl_map_t* map, void* key,
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

void** avl_map_keys(const avl_map_t* map) {
  void** keys = malloc(map->map->size * sizeof(*keys));

  int index = 0;
  _map_keys_aux(map->map->root, keys, &index);

  return keys;
}


map_iterator_t* avl_map_create_iterator(const avl_map_t* map) {
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


void avl_map_unref(avl_map_t* map) {
  if (map) {
    avl_erase_tree(map->map);
    free(map);
  }
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

void avl_map_dump(const avl_map_t* map) {
  printf("{ ");
  int padding = _max_key_size(map->map->root, map->key_as_string);
  _map_dump_aux(map->map->root, padding,
		map->key_as_string, map->data_as_string);
  printf("\b \n}\n");
}

void avl_map_dump_fast(const avl_map_t* map) {
  printf("{ ");
  _map_dump_aux(map->map->root, 0,
		map->key_as_string, map->data_as_string);
  printf("\b \n}\n");
}
