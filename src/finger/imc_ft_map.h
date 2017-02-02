#ifndef imc_ft_map_h
#define imc_ft_map_h

#include <stdio.h>
#include "fingertree.h"

typedef imc_key_t;
typedef imc_data_t;

typedef struct {
    ft* fgt;
    imc_key_t (*compose)(imc_key_t*, imc_key_t*);
} imc_ft_map_t;

//The following line provokes a compile error depending on the environment
imc_ft_map_t* imc_ft_map_create(imc_key_t*(*compose)(imc_key_t*, imc_key_t*));

int imc_ft_map_size(imc_ft_map_t* map);

imc_ft_map_t* imc_ft_map_update(imc_ft_map_t* map,
                             imc_key_t* key,
                             imc_data_t* data,
                             imc_data_t* prev_data);

imc_data_t* imc_ft_map_lookup(imc_ft_map_t* map,
                           imc_key_t* key);

imc_ft_map_t* imc_ft_map_remove(imc_ft_map_t* map,
                             imc_key_t* key,
                             imc_data_t** data);

imc_key_t** imc_ft_map_keys(imc_ft_map_t* map);

imc_ft_map_t* imc_ft_map_merge(imc_ft_map_t* orig_map,
                               imc_ft_map_t* app_map);

// iterator ?
typedef struct {
    imc_key_t key;
    imc_ft_map_t map;
} imc_ft_map_iterator_t;

imc_key_t imc_ft_map_next(imc_ft_map_iterator_t *iter);

/* user-side memory management */

int imc_ft_map_unref(imc_ft_map_t* map);

void imc_ft_map_dump(imc_ft_map_t* map);

#endif
