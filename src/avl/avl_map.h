#ifndef _AVL_MAP
#define _AVL_MAP

#include "avl.h"

typedef struct _avl_map_t {
  avl_tree* map;
} avl_map_t;

typedef struct _map_key_t map_key_t;
typedef struct _map_data_t map_data_t;
char* key_as_string(map_key_t* key);
char* data_as_string(map_data_t* data);
int key_compare(map_key_t* k1, map_key_t* k2);

avl_map_t* avl_map_create();

int avl_map_size (avl_map_t* map);

avl_map_t* avl_map_update(avl_map_t* map, map_key_t* key, map_data_t* data);

map_data_t* avl_map_lookup(avl_map_t* map, map_key_t* key);

avl_map_t* avl_map_remove(avl_map_t* map, map_key_t* key, map_data_t** data);

int avl_map_unref(avl_map_t* map);

void avl_map_dump(avl_map_t* vec);


#endif
