#include <stdio.h>
#include <stdin.h>
#include <string.h>
#include <errno.h>

#include "../debug.h"
#include "./rrb_vect.h"

/* --> rrb_vect.h */
/* TODO -> invariant , pre/post cond */
imc_rrb_t* imc_vector_concrete_create() {
    imc_rrb_t* vec = malloc(sizeof(imc_rrb_t));
    
    if(vec == NULL){
        LOG(1, "Allocation failure %s", strerror(errno));
        return NULL;
    }
    vec -> level = 1;
    vec -> refs = 1;
    vec -> element_count = 0;
    vec -> meta = NULL;
    vec -> childs = malloc(sizeof(imc_rrb_node_t) * ARRAY_SIZE);

    if(vec -> childs == NULL){
        LOG(1, "Allocation failure %s", strerror(errno));
        free(vec);
        return NULL;
    }
    return vec;

}

/* TODO -> invariant , pre/post cond */
int imc_vector_concrete_size(imc_rrb_t* vec) {
    return vec -> element_count;
}


