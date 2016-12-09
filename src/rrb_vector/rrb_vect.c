#include "rrb_vect.h"

/* Allocates an empty internal node for the rrb vector */
imc_rrb_intern_t* alloc_empty_internal_node(){
    imc_rrb_intern_t* root = malloc(sizeof(imc_rrb_intern_t));
    root -> level = 1;
    root -> refs = 1;
    root -> is_unbalenced = 0;
    root -> meta = malloc(sizeof(int) * ARRAY_SIZE);
    root -> childs = malloc(sizeof(imc_rrb_node_t) * ARRAY_SIZE);

    return root;
}

/* --> rrb_vect.h */
imc_rrb_t* imc_vector_concrete_create(){
    imc_rrb_t* rrb = malloc(sizeof(imc_rrb_t));
    rrb -> max_depth = MAX_DEPTH;
    rrb -> m = ARRAY_SIZE;
    rrb -> root = alloc_empty_internal_node();

    return rrb;
}





