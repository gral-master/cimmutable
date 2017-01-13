#include "imc_avl_vector.h"
#include <stdlib.h>

imc_vector_avl_t* imc_vector_avl_create(){
    imc_vector_avl_t * vect = malloc(sizeof(imc_vector_avl_t));
    vect->tree = NULL;
    vect->last_value = -1;
    return vect;
}

int imc_vector_avl_size(imc_vector_avl_t* vec){
    return vec->last_value + 1;
}

int int_comparator(imc_key_t *first_key, imc_key_t* second_key){
    if(first_key > second_key){
        return 1;
    }
    return first_key < second_key ? -1 : 0 ;
}

imc_vector_avl_t* imc_vector_avl_update(imc_vector_avl_t* vec, int index,
                                        imc_data_t* data){
    imc_avl_vector *new_version = malloc(sizeof(imc_avl_vector));
    imc_avl_node_t *prev_data=NULL;
    new_version->tree = imc_avl_insert(vec->tree, data, index,
                                       int_comparator, &prev_data);
    new_version->last_value = index > vec->last_value ? index : vec->last_value;

    return new_version;
}

imc_data_t* imc_vector_avl_lookup(imc_vector_avl_t* vec, int index){
    return imc_vector_avl_lookup(vec->tree, index, int_comparator);
}

/* stack operations */
int push_comparator(imc_key_t *first_key, imc_key_t* second_key){
    return first_key!=second_key?-1:0;
}

// add at the end <--- DOXYGENIZE PLEASE!
imc_vector_avl_t* imc_vector_avl_push(imc_vector_avl_t* vec,
			      imc_data_t* data){
    imc_avl_vector *new_version = malloc(sizeof(imc_avl_vector));
    imc_avl_node_t *prev_data=NULL;
    new_version->tree = imc_avl_insert(vec->tree, data, vec->last_value+1,
                                       push_comparator, &prev_data);
    new_version->size = vec->size + 1;
    new_version->last_value = vec->last_value+1;
    if(prev_data != NULL){
        printf("ERROR : push function disfunctionment!!");
    }

    return new_version;
}

imc_vector_avl_t* imc_vector_avl_pop(imc_vector_avl_t* vec,
			     imc_data_t** data){
    imc_avl_vector *new_version = malloc(sizeof(imc_avl_vector));
    new_version->tree = imc_avl_remove(vec->tree, vec->last_value,
                                    push_pop_comparator, &data);
    new_version->last_value = vec->last_value-1;
    if(prev_data == NULL){
        printf("ERROR : pop function disfunctionment!!");
    }

    return new_version;
}

int imc_vector_avl_split(imc_vector_avl_t* vec_in,
		     int index,
		     imc_vector_avl_t** vec_out1,
		     imc_vector_avl_t** vec_out2){

}

imc_vector_avl_t* imc_vector_avl_merge(imc_vector_avl_t* vec_front,
			       imc_vector_avl_t* vec_tail){

}

/* user-side memory management */

int imc_vector_avl_unref(imc_vector_avl_t* vec){
    imc_avl_unref(vec->tree);
    free(vec);
    return 1;
}

void imc_vector_avl_dump_rec(imc_avl_node_t* tree,
                          void (*print_key)(imc_key_t*),
                          void (*print_data)(imc_data_t*)){
    if (tree != NULL) {
        imc_avl_map_dump_rec(tree->left, print_key, print_data);
        printf(" (");
        print_key(tree.key);
        printf(" : ");
        print_data(tree->data);
        printf(") ");
        imc_avl_map_dump_rec(tree->right, print_key, print_data);
    }

}

void imc_vector_avl_dump(imc_avl_map_t* map,
                      void (*print_key)(imc_key_t*),
                      void (*print_data)(imc_data_t*)){
    printf("[");
    imc_avl_map_dump_rec(map->tree, print_key, print_data);
    printf("]\n");
}
