#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include "../debug.h"
#include "./rrb_vect.h"
#include "./rrb_checker.h"

/*******************************/
/* Creation and initialization */
/*******************************/

imc_rrb_t* imc_rrb_create() {
    // Allocates the vector
    imc_rrb_t* vec = malloc(sizeof(imc_rrb_t));
    if(vec == NULL){
        LOG(LOG_FATAL, "Allocation failure %s", strerror(errno));
        return NULL;
    }
    vec->floor = 1;
    // Fill the vector with inital values
    imc_rrb_init(vec);
    return vec;
}

imc_rrb_t* imc_rrb_create_leaf() {
    imc_rrb_t* vec = malloc(sizeof(imc_rrb_t));
    if(vec == NULL){
        LOG(LOG_FATAL, "Allocation failure %s", strerror(errno));
        return NULL;
    }
    vec -> floor = 0;
    imc_rrb_init(vec);
    return vec;
}

int imc_rrb_init(imc_rrb_t* vec) {
    vec->refs = 1;
    vec->length = 0;
    vec->meta = NULL;
    
    if(imc_rrb_is_leaf(vec)) {
        vec->node.data = calloc(ARRAY_SIZE, sizeof(imc_data_t*));
        if(vec->node.data == NULL){
            LOG(LOG_FATAL, "Allocation failure %s", strerror(errno));
            free(vec);
            return 0;
        }
    } else {
        vec->node.next = calloc(ARRAY_SIZE, sizeof(imc_rrb_t*));
        if(vec->node.next == NULL){
            LOG(LOG_FATAL, "Allocation failure %s", strerror(errno));
            free(vec);
            return 0;
        }
    }
    return 1;
}

/*****************/
/* rrb utilities */
/*****************/

int imc_rrb_size(imc_rrb_t* vec) {
    /* TODO : For leaves, length = non NULL data[i] nop ?? */
    if(vec == NULL) return 0;
    //if(vec->floor > 0){
    return vec->length;
    /*}
      int i, count = 0;
      imc_data_t** data = vec->node.data;
      for(i = 0; i < ARRAY_SIZE; i++){
      if(data[i] != NULL) count++;
      }
      return count;*/
}

int imc_rrb_is_full(imc_rrb_t* vec) {
    if(imc_rrb_is_balanced(vec) == 1) {
        return (vec->length == (32*pow(ARRAY_SIZE, vec->floor))) ? 1 : 0;
    } else {
        while(vec->floor > 1) {
            if(vec->meta[ARRAY_SIZE-1]==0) {
                return 0;
            }
        }
        vec = vec->node.next[ARRAY_SIZE-1];
        return (vec->node.data[ARRAY_SIZE-1]!=NULL) ? 1 : 0;
    }
}

int imc_rrb_is_balanced(imc_rrb_t* vec) {
    /* /!\ Is that always true ? What if the rrb has been rebalanced ? /!\ */
    return (vec->meta==NULL) ? 1 : 0;
}

int imc_rrb_is_leaf(imc_rrb_t* vec) {
    return (vec->floor==0) ? 1 : 0;
}

imc_rrb_t* imc_rrb_tail(imc_rrb_t* vec, int* res_index) {
    /* Return last non-NULL child and set res_index if not null */
    if(imc_rrb_is_leaf(vec)) {
        return NULL;
    } else {
        imc_rrb_t* res = NULL;
        for(int i = 0 ; i < ARRAY_SIZE ; i++) {
            if(vec->node.next[i]!=NULL) {
                res = vec->node.next[i];
                if(res_index != NULL) {
                    *res_index = i;
                }
            }
        }
        return res;
    }
}

imc_rrb_t* imc_rrb_head(imc_rrb_t* vec, int* res_index) {
    /* Return first non-NULL child and set res_index if not null */
    if(imc_rrb_is_leaf(vec)) {
        return NULL;
    } else {
        for(int i = 0 ; i < ARRAY_SIZE ; i++) {
            if(vec->node.next[i]!=NULL) {
                if(res_index != NULL) {
                    *res_index = i;
                }
                return vec->node.next[i];
            }
        }
    }
    return NULL;
}

imc_rrb_t* imc_rrb_new_root(imc_rrb_t* vec) {
    /* Add a new root to the top of a vector with vec as first child */
    /* Create a new root */
    imc_rrb_t* new_root = imc_rrb_create();
    new_root->length = vec->length;
    new_root->floor = vec->floor+1;
    /* vec is his first child */
    new_root->node.next[0] = vec;
    new_root->node.next[0]->refs += 1;

    return new_root;
}

imc_rrb_t* imc_rrb_new_root_alt(imc_rrb_t* vec) {
    /* Add a new root to the top of a vector with vec as last child */
    /* Create a new root */
    imc_rrb_t* new_root = imc_rrb_create();
    new_root->length = vec->length;
    new_root->meta = calloc(ARRAY_SIZE, sizeof(int));
    new_root->floor = vec->floor+1;
    /* vec is his first child */
    new_root->node.next[ARRAY_SIZE-1] = vec;
    new_root->node.next[ARRAY_SIZE-1]->refs += 1;
    new_root->meta[ARRAY_SIZE-1] = vec->length;

    return new_root;
}


void imc_rrb_increase_length(imc_rrb_t* vec, int index) {
    /* increase the length, and update the meta information */
    vec->length+=1;
    /* If the node is balanced, or if it is a leaf, meta is null */
    if(vec->meta != NULL) {
        for(int i = 0; i<ARRAY_SIZE; i++) {
            if(index <= vec->meta[i]) {
                vec->meta[i] += 1;
                if(i != ARRAY_SIZE-1) {
                    vec->meta[ARRAY_SIZE-1] += 1;
                }
                break;
            }
        }
    }
}

void imc_rrb_decrease_length(imc_rrb_t* vec, int index) {
    /* decrease the length, and update the meta information */
    vec->length-=1;
    /* If the node is balanced, or if it is a leaf, meta is null */
    if(vec->meta != NULL) {
        for(int i = 0; i<ARRAY_SIZE; i++) {
            if(index <= vec->meta[i]) {
                vec->meta[i] -= 1;
                if(i != ARRAY_SIZE-1) {
                    vec->meta[ARRAY_SIZE-1] -= 1;
                }
                break;
            }
        }
    }
}

int imc_rrb_subindex(imc_rrb_t* vec, int index) {
    int floor_index = 0;
    if(imc_rrb_is_balanced(vec)==1) { 
    //Si on est balanced, on sait tout de suite où aller
        floor_index = (index >> (int)(log2((double)ARRAY_SIZE) * vec->floor)) 
            & (ARRAY_SIZE-1);
    } else { //Si on est unbalanced, on doit chercher où aller
        while(vec->meta[floor_index] <= index) {
            floor_index++;
            if(floor_index==ARRAY_SIZE) { //We didn't find it
                return -1;
            }
        }
    }
    return floor_index;
}

imc_data_t* imc_rrb_get_leaf_no_copy(imc_rrb_t* vec, int index) {
    printf("lookup in a vector of size %d at index %d\n", vec->length, index);

    /* Go through the tree if the index is reachable, and return the data */
    int floor_index = 0;
    while(vec->floor!=0) {
        floor_index = imc_rrb_subindex(vec, index);
        if(floor_index == -1) {
            puts("index cannot be reached.\n");
            return NULL;
        } else {
            vec = vec->node.next[floor_index];
        }
    }
    floor_index = imc_rrb_subindex(vec, index);
    return vec->node.data[floor_index];
}

int imc_rrb_fill_fifo(imc_rrb_t* rrb, imc_rrb_t** fifo,
                      int i_fifo, int ignore){
    imc_rrb_t* tmp;
    for(int i = 0; i < elem_on_floor(rrb); i++){
        if(i != ignore){
            tmp = rrb->node.next[i];
            if(tmp != NULL){
                fifo[i_fifo] = tmp;
                tmp->refs++;
                i_fifo++;
            }
        }
    }
    return i_fifo;
}

/**************/
/* Xtra tools */
/**************/

char* concatc(char* str, char c) {
    /* Concat a char to a char* */
    size_t len = strlen(str);
    char *str2 = malloc(len + 1 + 1);
    strcpy(str2, str);
    str2[len] = c;
    str2[len + 1] = '\0';
    return str2;
}

/*******************/
/* Copy operations */
/*******************/

imc_rrb_t* imc_rrb_copy_leaf(imc_rrb_t* vec) {
    imc_rrb_t* vec_copy = imc_rrb_create_leaf();
    vec_copy -> floor = vec -> floor;
    vec_copy -> length =  vec -> length;
    vec_copy -> meta = vec -> meta; // A changer en allouant un nouveau tableau meta
    if(vec -> node.data != NULL) {
        for(int i = 0; i < ARRAY_SIZE ; i++) {
            vec_copy -> node.data[i] = vec -> node.data[i];
        }
    }
    return vec_copy;
}

imc_rrb_t* imc_rrb_copy(imc_rrb_t* vec) {
    imc_rrb_t* vec_copy = imc_rrb_create();
    vec_copy -> floor = vec -> floor;
    vec_copy -> length =  vec -> length;
    vec_copy -> meta = vec -> meta; // A changer en allouant un nouveau meta
    vec_copy -> refs = 1; //It is a copy, so we have one and only one ref to it
    if(vec -> node.next != NULL) {
        for(int i = 0; i < ARRAY_SIZE ; i++) {
            vec_copy -> node.next[i] = vec -> node.next[i];
            if(vec_copy -> node.next[i] != NULL) {
                vec_copy -> node.next[i] -> refs += 1;
            }
        }
    }
    return vec_copy;
}


/******************/
/* Push operation */
/******************/

imc_rrb_t* imc_rrb_push(imc_rrb_t* vec, imc_data_t* data) {
    if(imc_rrb_is_full(vec)==1) {
        return imc_rrb_push_full(vec, data);
    } else {
        return imc_rrb_push_not_full(vec, data);
    }
}

imc_rrb_t* imc_rrb_push_full(imc_rrb_t* vec, imc_data_t* data) {
 
    /* Make a new root with the vec as first child */
    imc_rrb_t* new_root = imc_rrb_new_root(vec);

    /* Create the path of nodes to add data in the desire leaf */
    new_root->node.next[1] = imc_rrb_create();
    vec = new_root->node.next[1];
    vec->floor = new_root->floor - 1;
 
    while(vec->floor > 1) {
        vec->node.next[0] = imc_rrb_create();
        vec->node.next[0]->floor = vec->floor - 1;
        vec->length = 1;
        vec = vec->node.next[0];
    }
    vec->length = 1;
    vec->node.next[0] = imc_rrb_create_leaf();
    vec = vec->node.next[0];
 
    /* Add the data to the leaf */
    vec->node.data[0] = data;
    new_root->length += 1;
 
    return new_root;
}
imc_rrb_t* imc_rrb_push_not_full(imc_rrb_t* vec, imc_data_t* data) {
    /* Make a new root */
    imc_rrb_t* new_root = imc_rrb_copy(vec);

    /* Go through the tree, increasing length and eventually meta by 1
       Until we have reached first floor */
    vec = new_root;
    int insert_index = vec->length;
    int sub_index;
    while(vec->floor != 1) {
        sub_index = imc_rrb_subindex(vec, insert_index);
        if(vec->node.next[sub_index] != NULL) {
            vec->node.next[sub_index]->refs -= 1;
            vec->node.next[sub_index] = imc_rrb_copy(vec->node.next[sub_index]);
        } else {
            vec->node.next[sub_index] = imc_rrb_create();
        }
        imc_rrb_increase_length(vec, insert_index);
        vec = vec->node.next[sub_index];
    }

    /* First floor, we copy the leaf (floor 0) if it exists, or create it */
    sub_index = imc_rrb_subindex(vec, insert_index);

    if(vec->node.next[sub_index] != NULL) {
        vec->node.next[sub_index]->refs--;
        vec->node.next[sub_index] =
            imc_rrb_copy_leaf(vec->node.next[sub_index]);
    } else {
        vec->node.next[sub_index] = imc_rrb_create_leaf();
    }
    imc_rrb_increase_length(vec, insert_index);
    vec = vec->node.next[sub_index];

    /* We push the new data */
    sub_index = imc_rrb_subindex(vec, insert_index);
    vec->node.data[sub_index] = data;
    imc_rrb_increase_length(vec, insert_index);
    return new_root;
}

/*****************/
/* Pop operation */
/*****************/

imc_rrb_t* imc_rrb_pop(imc_rrb_t* vec, imc_data_t** data) {
    //TODO : Si la branche devient inutile, il faut la supprimer. 
    // Idem pour un étage, ça ne coûte pas cher !
    // 1) We go to the desire data, copying the path
    // 2) At floor 1, we copy the data node if and only if the looking data
    // is not in first position
    // 3) While copying the data, we remove the last value
    if(vec->length<=0) {
        //puts("vector is already empty !\n");
        return vec;
    }

    /* We make a new root */
    imc_rrb_t* new_root = imc_rrb_copy(vec);

    /* We go through the tree, decreasing length, increasing refs, 
       until floor 1*/
    vec = new_root;
    int sub_index = 0;
    int last_index = new_root->length-1;
    while(vec->floor!=1) {
        sub_index = imc_rrb_subindex(vec, last_index); // We want the last elem
        vec->node.next[sub_index]->refs -= 1; //Increased by previous copy
        vec->node.next[sub_index] = imc_rrb_copy(vec->node.next[sub_index]);
        imc_rrb_decrease_length(vec, sub_index);
        vec = vec->node.next[sub_index];
    }
    //floor 1 : vec's child is a leaf.
    sub_index = imc_rrb_subindex(vec, last_index);
    vec->node.next[sub_index]->refs--;
    vec->node.next[sub_index] = imc_rrb_copy_leaf(vec->node.next[sub_index]);
    imc_rrb_decrease_length(vec, sub_index);
    vec = vec->node.next[sub_index];
    //floor 0 : vec is a leaf
    sub_index = imc_rrb_subindex(vec, last_index);
    *data = vec->node.data[sub_index];
    vec->node.data[sub_index] = NULL;
    imc_rrb_decrease_length(vec, sub_index);
    return new_root;
}

/********************/
/* Update operation */
/********************/

imc_rrb_t* imc_rrb_update(imc_rrb_t* vec, int index, imc_data_t* data) {
    puts("\nUpdate in a vector : ");
    /* Make a new root */
    imc_rrb_t* new_root = imc_rrb_copy(vec);

    /* Go through the tree, copying the path, until first floor */
    vec = new_root;
    int sub_index;
    while(vec->floor != 1) {
        sub_index = imc_rrb_subindex(vec, index);
        if(vec->node.next[sub_index] != NULL) {
            vec->node.next[sub_index]->refs -= 1;
            vec->node.next[sub_index] = imc_rrb_copy(vec->node.next[sub_index]);
        } else {
            //Should not happen
            //TODO : Free previous
            return vec;
        }
        vec = vec->node.next[sub_index];
    }

    /* First floor : we copy the leaf (floor 0) and also if the leaf exists */
    sub_index = imc_rrb_subindex(vec, index);
    if(vec->node.next[sub_index]!=NULL) {
        vec->node.next[sub_index]->refs -= 1;
        vec->node.next[sub_index] = 
            imc_rrb_copy_leaf(vec->node.next[sub_index]);
    } else {
        //Should not happen
        //TODO : Free previous
        return vec;
    }
    vec = vec->node.next[sub_index];
 
    /* We update the data */
    sub_index = imc_rrb_subindex(vec, index);
    vec->node.data[sub_index] = data;

    return new_root;
}

/********************/
/* Lookup operation */
/********************/

imc_data_t* imc_rrb_lookup(imc_rrb_t* vec, int index) {
    /* Go through the tree if the index is reachable, and return the data */
    int floor_index = 0;
    while(vec->floor!=0) {
        floor_index = imc_rrb_subindex(vec, index);
        if(floor_index == -1) {
            return NULL;
        } else {
            vec = vec->node.next[floor_index];
        }
    }
    floor_index = imc_rrb_subindex(vec, index);
    return vec->node.data[floor_index];
}

/*******************/
/* Split operation */
/*******************/

int imc_rrb_split(imc_rrb_t* vec_in, int index, imc_rrb_t** vec_out1,
                  imc_rrb_t** vec_out2) {
    imc_rrb_build_left(vec_in, *vec_out1, index);
    imc_rrb_build_right(vec_in, *vec_out2, index);
    return 0;
}

void imc_rrb_build_left(imc_rrb_t* vec_in, imc_rrb_t* left,
                        int index){
    int i, split_index = imc_rrb_subindex(vec_in, index);
    
    if(left == NULL){
        left = imc_rrb_create();
    }
    
    left->floor = vec_in->floor;
    if(vec_in->floor == 0){
        for(i = 0; i <= split_index; i++){
            left->node.data[i] = vec_in->node.data[i];
        }
        left->length = split_index+1;
        return;
    }
    int is_balanced = imc_rrb_is_balanced(vec_in);
    if(is_balanced == 0) left->meta = malloc(sizeof(int) * ARRAY_SIZE);

    for(i = 0; i < split_index; i++){
        left->node.next[i] = vec_in->node.next[i];
        if(left->node.next[i] != NULL) {
            left->node.next[i]->refs++;
        }
        if(is_balanced == 0) left->meta[i] = vec_in->meta[i];
    }
    if(vec_in->floor > 1) left->node.next[split_index] = imc_rrb_create();
    else left->node.next[split_index] = imc_rrb_create_leaf();
    
    imc_rrb_build_left(vec_in->node.next[split_index],
                       left->node.next[split_index], index);
    if(is_balanced == 0){
        if(split_index == 0){
            left->meta[split_index] = imc_rrb_size(left->node.next[split_index]);
        }
        else{
            left->meta[split_index] =
                imc_rrb_size(left->node.next[split_index]) 
                + left->meta[split_index-1];
        }
        left->length = left->meta[split_index];
    }
    else{
        int count = 0;
        for(i = 0; i <= split_index; i++) count += imc_rrb_size(left->node.next[i]);
        left->length = count; 
    }
}

void imc_rrb_build_right(imc_rrb_t* vec_in, imc_rrb_t* right,
                         int index){
    /* Looking where we need to split in the node */
    int i, j, split_index = imc_rrb_subindex(vec_in, index);
    if(right == NULL) right = imc_rrb_create();
    right->floor = vec_in->floor;
    /* If vec_in is a leaf, and we try to split at the end, we don't need it */
    if(vec_in->floor == 0){
        if(split_index == ARRAY_SIZE-1){
            right->length = -1;
            return;
        }
        /* Else, we copy the data in the leaf */
        for(j = 0, i = split_index+1; i < ARRAY_SIZE; i++, j++){
            right->node.data[j] = vec_in->node.data[i];
        }
        right->length = j;
        return;
    }
    right->meta = malloc(sizeof(int) * ARRAY_SIZE);

    for(i = split_index+1; i < ARRAY_SIZE; i++){
        right->node.next[i] = vec_in->node.next[i];
        if(right->node.next[i]!=NULL) {
            right->node.next[i]->refs++;
        }
    }
    if(vec_in->floor > 1) right->node.next[split_index] = imc_rrb_create();
    else right->node.next[split_index] = imc_rrb_create_leaf();

    imc_rrb_build_right(vec_in->node.next[split_index],
                        right->node.next[split_index], index);

    if(right->node.next[split_index]->length == -1){
        imc_rrb_unref(right->node.next[split_index]);
        right->node.next[split_index] = NULL;
        int all_null = right->node.next[0] == NULL;
        for(i = 1; i < ARRAY_SIZE; i++){
            all_null = all_null && right->node.next[i] == NULL;
            if(!all_null) break;
        }
        if(all_null){
            right->length = -1;
            return;
        }
    }
    for(i = 0; i < ARRAY_SIZE; i++){
        if(i < split_index) right->meta[i] = 0;
        if(i == split_index){
            right->meta[i] = imc_rrb_size(right->node.next[split_index]);
        }
        if(i > split_index){
            right->meta[i] = right->meta[i-1] + 
                imc_rrb_size(right->node.next[i]);
        }
    }
    right->length = right->meta[ARRAY_SIZE-1];
}


/*******************/
/* Merge operation */
/*******************/

imc_rrb_t* imc_rrb_merge(imc_rrb_t* vec_front, imc_rrb_t* vec_tail) {
    imc_rrb_t* new_rrb = NULL;
    int ind, j = 0, i = 0;
    if(vec_front->floor > vec_tail->floor){
        while(vec_front->floor - vec_tail->floor > 0){
            new_rrb = imc_rrb_new_root(vec_tail);
            if(i == 0){ i++;}
            else new_rrb->node.next[0]->refs--;
            vec_tail = new_rrb;
        }
    }
    if(vec_front->floor < vec_tail->floor){
        while(vec_tail->floor - vec_front->floor > 0){  
            new_rrb = imc_rrb_new_root_alt(vec_front);
            if(i == 0){ i++;}
            else new_rrb->node.next[ARRAY_SIZE-1]->refs--;
            vec_front = new_rrb;
        }
    }
    i = 0;
    //S1 et S2 de la taille des 2
    int size1 = vec_front->floor+1;
    imc_rrb_t** path1 = malloc(size1 * sizeof(imc_rrb_t*));
    int* path_id1 = malloc(size1 * sizeof(int));
    int size2 = vec_tail->floor+1;
    imc_rrb_t** path2 = malloc(size2 * sizeof(imc_rrb_t*));
    int* path_id2 = malloc(size2 * sizeof(int));
    imc_rrb_t* vec = vec_front;

    /* On remplit les tableaux pour les chemins */
    do {
        path1[i] = vec;
        vec = imc_rrb_tail(vec, &ind);
        path_id1[i] = ind;
        i++;
    } while(i != size1);
    vec = vec_tail;
    i = 0;
    do {
        path2[i] = vec;
        vec = imc_rrb_head(vec, &ind);
        path_id2[i] = ind;
        i++;
    } while(i != size2);

    /* On merge à partir des feuilles */
    i = size1-1;
    j = size2-1;
    vec = NULL;
    while(i>=0 && j>=0) {
        if(i==0 || j ==0) {
            vec = compress(path1[i], vec, path2[j], 
                           path_id1[i], path_id2[j], 1);
        } else {
            vec = compress(path1[i], vec, path2[j], 
                           path_id1[i], path_id2[j], 0);
        }
        i--; j--;
    }
    if(new_rrb != NULL) imc_rrb_unref(new_rrb);
    free(path1);
    free(path2);
    free(path_id1);
    free(path_id2);
    return vec;
}

imc_rrb_t* compress(imc_rrb_t* left, imc_rrb_t* mid, imc_rrb_t* right, 
                    int ignore1, int ignore2, int last_step) {
    if(left->floor == 0) {
        return imc_rrb_merge_leaves(left, right);
    } else {
        return imc_rrb_merge_nodes(left, mid, right, ignore1, 
                                   ignore2, last_step);
    }
}

int elem_on_floor(imc_rrb_t* rrb){
    int res = 0;
    for(int i = 0; i < ARRAY_SIZE; i++){
        if(rrb->node.next[i] != NULL) res++;
    }
    return res;
}

imc_rrb_t* imc_rrb_merge_nodes(imc_rrb_t* left, imc_rrb_t* middle, 
                               imc_rrb_t* right, int ignore1, 
                               int ignore2, int last_step) {
    int i, j = 0, index_fifo = 0;
    imc_rrb_t* node1 = imc_rrb_create();
    imc_rrb_t* node2 = NULL;
    imc_rrb_t* node3 = NULL;
    int size = elem_on_floor(left) + elem_on_floor(middle) 
        + elem_on_floor(right) - 2;
    imc_rrb_t** fifo = malloc(sizeof(imc_rrb_t*) * size);
    if(size > ARRAY_SIZE) node2 = imc_rrb_create();
    if(size > ARRAY_SIZE*2) node3 = imc_rrb_create();
    index_fifo = imc_rrb_fill_fifo(left, fifo, index_fifo, ignore1);
    index_fifo = imc_rrb_fill_fifo(middle, fifo, index_fifo, -1);
    printf("size fifo : %d\n", size);
    index_fifo = imc_rrb_fill_fifo(right, fifo, index_fifo, ignore2);
    for(i = 0; i < size; i++){
        if(i < ARRAY_SIZE){
            node1->node.next[j] = fifo[i];
            //node1->node.next[j]->refs++;
            node1->length += imc_rrb_size(node1->node.next[j]);
        }
        if(i >= ARRAY_SIZE && i < ARRAY_SIZE*2){
            if(i == ARRAY_SIZE) j = 0;
            node2->node.next[j] = fifo[i];
            //node2->node.next[j]->refs++;
            node2->length += imc_rrb_size(node2->node.next[j]);
        
        }
        if(i >= ARRAY_SIZE*2){
            if(i == ARRAY_SIZE*2) j = 0;
            node3->node.next[j] = fifo[i];
            //node3->node.next[j]->refs++;
            node3->length += imc_rrb_size(node3->node.next[j]);
        }
        j++;
    }
    imc_rrb_t* root = node1;
    imc_rrb_unref(middle);
    free(fifo);
    if((last_step == 1) && (size <= ARRAY_SIZE*2)){
        return root;
    } 
    else {
        root = imc_rrb_new_root(node1);
        root->node.next[0]->refs = 1;
        root->length = 1;
        if(size > ARRAY_SIZE){
            root->node.next[1] = node2;
            root->node.next[1]->refs = 1;
            root->length = 2;
        }
        
        if(size > ARRAY_SIZE*2){
            root->node.next[2] = node2;
            root->node.next[2]->refs = 1;
            root->length = 3;
        }
        return root;    
    }
}

imc_rrb_t* imc_rrb_merge_leaves(imc_rrb_t* vec1, imc_rrb_t* vec2) {
    int i, j, k, size = vec1->length + vec2->length;
    imc_rrb_t* leaf1 = imc_rrb_create_leaf();
    imc_rrb_t* leaf2 = NULL;
    if(size > ARRAY_SIZE) leaf2 = imc_rrb_create_leaf();
    /* Data merging */
    for(i = 0, j = 0, k = 0; i < size; ){
        if(i < ARRAY_SIZE){
            if(i < vec1->length) leaf1->node.data[i] = vec1->node.data[i];
            else {
                leaf1->node.data[i] = vec2->node.data[j];
                j++; //Iterateur sur vec2
            }
        }
        else {
            leaf2->node.data[k] = vec2->node.data[j];
            j++; //Iterateur sur vec2
            k++; //Iterateur sur leaf2
        }
        i++; //Iterateur global, et iterateur sur vec1/leaf1
    }
    /* Root adding */
    imc_rrb_t* root = imc_rrb_new_root(leaf1);
    root->node.next[0]->refs = 1;
    root->node.next[0]->length = i - k;
    root->length = size;
    if(leaf2 != NULL){
        root->node.next[1] = leaf2;
        root->node.next[1]->refs = 1;
        root->node.next[1]->length = k;  
    }
    return root;
}
/*******************/
/* Unref operation */
/*******************/

int imc_rrb_unref(imc_rrb_t* vec) {
    if(vec == NULL) { return 0; }
    int i;
    vec->refs--;
    if(vec->refs == 0){
        if(vec->floor == 0){
            free(vec->node.data);
            free(vec);
            return 1;
        }
        imc_rrb_t** next = vec->node.next;
        for(i = 0; i < ARRAY_SIZE; i++){
            if(next[i] != NULL) imc_rrb_unref(next[i]);
        }
        if(!imc_rrb_is_balanced(vec)) free(vec->meta);
        free(vec->node.next);
        free(vec);
        return 1;
    }
    return 0;
}

/********************/
/* Print operations */
/********************/

void imc_rrb_dump(imc_rrb_t* vec) {
    puts("It is a really bad idea to print a rrb in your terminal.\n");
    puts("Please use imc_rrb_emit instead..\n");
}

void imc_rrb_emit(imc_rrb_t* vec, const char* path, char* (*print)(imc_data_t*)){
    if(vec==NULL) {
        return;
    }
    FILE *fp;
    fp = fopen(path, "w+");
    //initialisation :
    fprintf(fp, "digraph g {\n");
    fprintf(fp, "node [shape = record,height=.1];\n");
    int yes = 1, no = 0;
    int do_you_want_refs_in_print = yes;
    emit_node(vec, NULL, "", fp, print, do_you_want_refs_in_print);
    fprintf(fp, "}\n");
    fclose(fp);
}

//On pourrait faire un emit_node_compact ?
void emit_node(imc_rrb_t* vec, char* from, char* prefix, FILE* fp,
               char* (*print)(imc_data_t*), int refs) {
    /* added i (as identifier) to the prefix, because of a graphviz bug :
     * https://rt.cpan.org/Public/Bug/Display.html?id=105171
     * Please use after that dot -Tsvg INPUT.dot -o OUTPUT.svg
     * and use a good visualizer if your graph is large.
     */
    fprintf(fp, "node_%s[label = \"", prefix);
    //current node
    char suffix;
    char *id;
    //if intern node, draw it with all its cells
    if(vec->floor != 0) {
        for(int i =0; i<ARRAY_SIZE; i++) {
            suffix = i<10?i+48:i+55;
            if(vec->node.next[i] != NULL) {
                //printf("prefix : %s\n", prefix);
                //printf("suffix : %c\n", suffix);
                id = concatc(prefix, suffix);
                fprintf(fp, "<i%s> %d", id, i);
                free(id);
                if(refs==1) fprintf(fp, ":%d", vec->refs);
            } else {
                id = concatc(prefix, suffix);
                fprintf(fp, "<i%s> ", id);
                free(id);
            }
            if(i != ARRAY_SIZE-1) {
                fprintf(fp, "| ");
            }
        }
        fprintf(fp, "\"];\n");
        //Draw arrow from parent to first cell, if it is not the root
        if(from != NULL) {
            fprintf(fp, "%s->\"node_%s\":i%s0;\n", from, prefix, prefix);
        }
        //recursive calls to draw children
        for(int i =0; i<ARRAY_SIZE; i++) {
            if(vec->node.next[i] != NULL) {
                suffix = i<10?i+48:i+55;
                char* str_from = malloc(sizeof(char) * 100);
                id = concatc(prefix, suffix);
                sprintf(str_from, "\"node_%s\":i%s", prefix, id);
                emit_node(vec->node.next[i], str_from, id, fp, print, refs);
                free(id);
                free(str_from);
            }
        }
        //If it is a leaf
    } else {
        // draw the leaf and its cells
        for(int i =0; i<ARRAY_SIZE; i++) {
            suffix = i<10?i+48:i+55;
            if(vec->node.data[i] != NULL) {
                id = concatc(prefix, suffix);
                char* data = print(vec->node.data[i]);
                fprintf(fp, "<i%s> %s ", id, data);
                free(data);
                free(id);
            } else {
                id = concatc(prefix, suffix);
                fprintf(fp, "<i%s> ", id);
                free(id);
            }
            if(i != ARRAY_SIZE-1) {
                fprintf(fp, "|");
            }
        }
        fprintf(fp, "\"];\n");
        if(from != NULL) {
            fprintf(fp, "%s->\"node_%s\":i%s0;\n", from, prefix, prefix);
        }
    }
}
