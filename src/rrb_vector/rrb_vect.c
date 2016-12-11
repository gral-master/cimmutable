#include <stdio.h>
#include <stdin.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
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

imc_vector_t* imc_vector_concrete_update(imc_vector_t* vec, int index, imc_data_t* data) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return NULL;
}

imc_data_t* imc_vector_concrete_lookup(imc_vector_t* vec, int index) {
  /* preconditions */
  if(index<0) {
    LOG(LOG_ERR, "Illegal access attempt with negative index %d.", index);
    return NULL;
  }
  if(vec==NULL) {
    //TODO : Regarder la dernière case de la meta, pour savoir si on va faire un accès trop grand
    LOG(LOG_ERR, "Illegal access attempt with positive index %d || Illegal lookup attempt in a NULL vector.", index);
    return NULL;
  }
  /* invariant */
  if(vec->level < 1) {
    LOG(LOG_FATAL, "Illegal data structure state : negative level %d.", level);
    return NULL;
  }
  /* algorithm */
  int level_index = 0;
  while(vec->level!=1) {
    level_index = imc_vector_concrete_subindex(vec, index);
    if(level_index != -1) {
      vec = vec->childs[level_index];
    } else {
      return NULL;
    }
  }
  level_index = imc_vector_concrete_subindex(vec, index);
  return vec->data[level_index];
}

/* stack operations */

// add at the end <--- DOXYGENIZE PLEASE!
imc_vector_t* imc_vector_concrete_push(imc_vector_t* vec, imc_data_t* data) {
  /* preconditions */
  if(data==NULL) {
    LOG(LOG_ERR, "Illegal push attempt NULL data.");
    return NULL;
  }
  if(vec==NULL) {
    //TODO : Regarder la dernière case de la meta, pour savoir si on va faire un accès trop grand
    LOG(LOG_ERR, "Illegal push attempt in a NULL vector.", index);
    return NULL;
  }
  /* invariant */
  if(vec->level < 1) {
    LOG(LOG_FATAL, "Illegal data structure state : negative level %d.", level);
    return NULL;
  }

  //algorithm
  imc_vector_t* new_root;
  if(imc_vector_concrete_full(vec==1)) {
    //New root with vec as first child
    new_root = imc_vector_concrete_new_root(vec);
    //Create the path to add data
    new_root->childs[1] = imc_vector_concrete_create();
    vec = new_root->childs[1];
    vec->level = new_root->level - 1;
    vec->element_count = 1;
    while(vec->level != 1) {
      vec->childs[0] = imc_vector_concrete_create();
      vec->childs[0]->level = vec->level - 1;
      vec = vec->childs[0];
      vec->element_count = 1;
    }
    vec->childs[0] = imc_vector_concrete_create_leaf();
    vec->childs[0]->data[0] = data;
    new_root->element_count += 1;
  } else {
    new_root = imc_vector_concrete_copy(vec); // TODO : should also update refs to childs
    vec = new_root;
    int sub_index;
    while(vec->level != 1) {
       sub_index = imc_vector_concrete_subindex(vec, new_root->element_count); // On veut ajouter à l'indice nb_element
       vec->childs[sub_index]->refs -= 1; //has been up by previous copy, we need to do -1
       vec->childs[sub_index] = imc_vector_concrete_copy(vec->childs[sub_index]); // This function updates refs to childs
       vec = vec->childs[sub_index];
    }
    vec->childs[sub_index] = imc_vector_concrete_copy_leaf(vec->childs[sub_index]); //This should copy the data
    vec = vec->childs[sub_index];
    sub_index = imc_vector_concrete_subindex(vec, new_root->element_count); // On veut ajouter à l'indice nb_element
    vec->data[sub_index] = data;
  }
  return new_root;
}

imc_vector_t* imc_vector_concrete_pop(imc_vector_t* vec, imc_data_t** data) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return NULL;
}

int imc_vector_concrete_split(imc_vector_t* vec_in, int index, imc_vector_t** vec_out1, imc_vector_t** vec_out2) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return -1;
}

imc_vector_t* imc_vector_concrete_merge(imc_vector_t* vec_front, imc_vector_t* vec_tail) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return NULL;
}

/* user-side memory management */

int imc_vector_concrete_unref(imc_vector_t* vec) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return -1;
}

void imc_vector_concrete_dump(imc_vector_t* vec) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return;
}

/* utils */
void imc_vector_concrete_emit(imc_vector_t* vec) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return;
}

/* return 1 if the vector is full */
int imc_vector_concrete_full(imc_vector_t* vec) {
  if(imc_vector_concrete_balanced(vec) == 1) {
    return (vec->element_count == (pow(ARRAY_SIZE, vec->level))) ? 1 : 0;
  } else {
    while(vec->level > 1) {
      if(vec->meta[ARRAY_SIZE-1]==0) {
        return 0;
      }
    }
    vec = vec->childs[ARRAY_SIZE-1];
    return (vec->data[ARRAY_SIZE-1]!=NULL) ? 1 : 0;
  }
}

/* Add a new root to the top of a vector */
imc_vector_t* imc_vector_concrete_new_root(imc_vector_t* vec) {
  imc_vector_t* new_root = NULL;
  new_root = imc_vector_concrete_create();
  new_root->childs[0] = vec;
  vec->refs+=1;
  new_root->element_count = vec->element_count;
  new_root->level = vec->level+1;
  return new_root;
}

/* return 1 if the vector is balanced */
int imc_vector_concrete_balanced(imc_vector_t* vec) {
  /* /!\ Is that always true ? What if the rrb has been rebalanced ? /!\ */
  return (vec->meta==NULL) ? 1 : 0;
}

/* return the subindex, i.e. the subindex you may choose at your current
   level to go to the vector index */
int imc_vector_concrete_subindex(imc_vector_t* vec, int index) {
  int level_index = 0;
  if(imc_vector_concrete_balanced(vec)==1) { //Si on est balanced, on sait tout de suite où aller
    level_index = (index >> (log2((double)ARRAY_SIZE) * vec->level)) & (ARRAY_SIZE-1);
  } else { //Si on est unbalanced, on doit chercher où aller
    while(vec->meta[level_index] <= index) {
      level_index++;
      if(level_index==ARRAY_SIZE) { //We didn't find it
        return -1;
      }
    }
  }
  return level_index;
}

imc_vector_t* imc_vector_concrete_copy_leaf(imc_vector_t* vec) {
  //TODO: implementation
  return NULL;
}

imc_vector_t* imc_vector_concrete_copy(imc_vector_t* vec) {
  //TODO: implementation
  return NULL;
}

imc_vector_t* imc_vector_concrete_create_leaf() {
  //TODO: implementation
  return NULL;
}
