#ifndef _AVL_MAP
#define _AVL_MAP

#include "avl.h"

typedef struct _avl_map_t {
  avl_tree* map;
  char* (*key_as_string)(void*);
  char* (*data_as_string)(void*);
} avl_map_t;


typedef struct _map_key_t map_key_t;
typedef struct _map_data_t map_data_t;
int key_compare(map_key_t* k1, map_key_t* k2);


typedef struct _map_iterator_t {
  avl_node* current;
  struct _map_iterator_t* next;
} map_iterator_t;

avl_map_t* avl_map_create(char* (*key_as_string)(void*),
			  char* (*data_as_string)(void*),
			  int (*key_compare)(void*,void*));

int avl_map_size (avl_map_t* map);

avl_map_t* avl_map_update(avl_map_t* map, void* key, void* data);

void* avl_map_lookup(avl_map_t* map, void* key);

avl_map_t* avl_map_remove(avl_map_t* map, void* key, void** data);

void** avl_map_keys(avl_map_t* map);

map_iterator_t* avl_map_create_iterator(avl_map_t* map);

int avl_map_iterator_next(map_iterator_t** iterator,
			  void** key, void** data);
			    
int avl_map_unref(avl_map_t* map);

void avl_map_dump(avl_map_t* vec);


#endif
