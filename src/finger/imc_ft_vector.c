#include "imc_ft_vector.h"
#include "fingertree.h"
#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

imc_ft_vector_t* imc_ft_vector_create() {
    return create_empty();
}

int imc_ft_vector_size(imc_ft_vector_t* vec) {
    return vec->size;
}

imc_ft_vector_t* imc_ft_vector_update(imc_ft_vector_t* vec,
                                   int index,
                                   imc_data_t* data) {
    #ifdef CHECK_SAFETY
    assert (index < vec->size);
    #endif
    
    split s = ft_split(vec, index);
    
    list* l = create_lempty();
    l = add(create_data_node(data), l);
    ft*tmp=concat_w_middle(s.ft1, l, s.ft2);
    
    ft_unref(s.ft1);
    ft_unref(s.ft2);
    node_unref(s.node);
    return tmp;
}

imc_data_t* imc_ft_vector_lookup(imc_ft_vector_t* vec,
                              int index) {
    return ft_lookup(vec, index);
}

/* stack operations */
imc_ft_vector_t* imc_ft_vector_push(imc_ft_vector_t* vec,
                                 imc_data_t* data) {
    return ft_add(data, vec, 1);
}

imc_ft_vector_t* imc_ft_vector_pop(imc_ft_vector_t* vec,
                                imc_data_t** data) {
    view v = ft_delete(vec, 1);
    *data = v.elem->true_node->data;
    return v.fg;
}

int imc_ft_vector_split(imc_ft_vector_t* vec_in,
                     int index,
                     imc_ft_vector_t** vec_out1,
                     imc_ft_vector_t** vec_out2) {
    if (vec_in->type == EMPTY_TYPE) {
      *vec_out1 = imc_ft_vector_create();
      *vec_out2 = imc_ft_vector_create();
      return EXIT_SUCCESS;
    }
    
    split s = ft_split(vec_in, index);
    ft* empty_tree = create_empty();
    list* l = create_lempty();
    l = add(s.node, l);
    
    *vec_out1 = concat_w_middle(s.ft1, l, empty_tree);
    *vec_out2 = s.ft2;
    ft_unref(empty_tree);
    ft_unref(s.ft1);
    node_unref(s.node);
    
    return EXIT_SUCCESS;
}

imc_ft_vector_t* imc_ft_vector_merge(imc_ft_vector_t* vec_front,
                                  imc_ft_vector_t* vec_tail) {
  
    return ft_concat(vec_front, vec_tail);
}

/* user-side memory management */

int imc_ft_vector_unref(imc_ft_vector_t* vec) {
    ft_unref(vec);
    
    return EXIT_SUCCESS;
}

void imc_ft_vector_dump(imc_ft_vector_t* vec) {
    ft_display(vec);
}
