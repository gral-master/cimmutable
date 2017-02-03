#ifndef imc_ft_vector_h
#define imc_ft_vector_h

#include "fingertree.h"

typedef ft imc_ft_vector_t;
typedef imc_data_t;

imc_ft_vector_t* imc_ft_vector_create();

int imc_ft_vector_size(imc_ft_vector_t* vec);

imc_ft_vector_t* imc_ft_vector_update(imc_ft_vector_t* vec,
                                int index,
                                imc_data_t* data);

imc_data_t* imc_ft_vector_lookup(imc_ft_vector_t* vec,
                              int index);

/* stack operations */

imc_ft_vector_t* imc_ft_vector_push(imc_ft_vector_t* vec,
                                 imc_data_t* data);

imc_ft_vector_t* imc_ft_vector_pop(imc_ft_vector_t* vec,
                                imc_data_t** data);

int imc_ft_vector_split(imc_ft_vector_t* vec_in,
                     int index,
                     imc_ft_vector_t** vec_out1,
                     imc_ft_vector_t** vec_out2);

imc_ft_vector_t* imc_ft_vector_merge(imc_ft_vector_t* vec_front,
                                  imc_ft_vector_t* vec_tail);

/* user-side memory management */

int imc_ft_vector_unref(imc_ft_vector_t* vec);

void imc_ft_vector_dump(imc_ft_vector_t* vec);

#endif
