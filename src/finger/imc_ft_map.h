#ifndef imc_ft_map_h
#define imc_ft_map_h

#include <stdio.h>
#include "fingertree.h"

typedef struct imc_avl_map_t{
    ft* fgt;
    imc_key_t (*compose)(imc_key_t*, imc_key_t*);
} imc_map_t;
typedef imc_key_t;
typedef imc_data_t;

//The following line provokes a compile error depending on the environment
//imc_map_t* imc_map_create((*compose)(imc_key_t*, imc_key_t*));

int imc_map_size(imc_map_t* map);

imc_map_t* imc_map_update(imc_map_t* map,
                          imc_key_t* key,
                          imc_data_t* data,
                          imc_data_t* prev_data);

imc_data_t* imc_map_lookup(imc_map_t* map,
                           imc_key_t* key);

imc_map_t* imc_map_remove(imc_map_t* map,
                          imc_key_t* key,
                          imc_data_t** data);

imc_key_t** imc_map_keys(imc_map_t* map);

imc_map_t* imc_vector_merge(imc_map_t* orig_map,
                            imc_map_t* app_map);

// iterator ?
typedef struct {
    imc_key_t key;
    imc_map_t map;
} imc_map_iterator_t;

imc_key_t imc_map_next(imc_map_iterator_t *iter);

/* user-side memory management */

int imc_map_unref(imc_map_t* map);

void imc_map_dump(imc_map_t* map);

#endif
