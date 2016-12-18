#include <stdio.h>
#include <stdlib.h>
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
        LOG(LOG_FATAL, "Allocation failure %s", strerror(errno));
        return NULL;
    }
    vec -> level = 1;
    vec -> refs = 1;
    vec -> element_count = 0;
    vec -> meta = NULL;
    vec -> children.subtrees = malloc(sizeof(imc_rrb_t) * ARRAY_SIZE);

    if(vec -> children.subtrees == NULL){
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

imc_rrb_t* imc_vector_concrete_update(imc_rrb_t* vec, int index, imc_data_t* data) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return NULL;
}

imc_data_t* imc_vector_concrete_lookup(imc_rrb_t* vec, int index) {
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
    LOG(LOG_FATAL, "Illegal data structure state : negative level %d.", vec->level);
    return NULL;
  }
  /* algorithm */
  int level_index = 0;
  while(vec->level!=1) {
    level_index = imc_vector_concrete_subindex(vec, index);
    if(level_index != -1) {
      vec = vec->children.subtrees[level_index];
    } else {
      return NULL;
    }
  }
  level_index = imc_vector_concrete_subindex(vec, index);
  return vec->children.data[level_index];
}

/* stack operations */

// add at the end <--- DOXYGENIZE PLEASE!
imc_rrb_t* imc_vector_concrete_push(imc_rrb_t* vec, imc_data_t* data) {
  puts("trying to push...\n");
  /* preconditions */
  if(data==NULL) {
    LOG(LOG_ERR, "Illegal push attempt NULL data.");
    return NULL;
  }
  if(vec==NULL) {
    //TODO : Regarder la dernière case de la meta, pour savoir si on va faire un accès trop grand
    LOG(LOG_ERR, "Illegal push attempt in a NULL vector.");
    return NULL;
  }
  /* invariant */
  if(vec->level < 1) {
    LOG(LOG_FATAL, "Illegal data structure state : negative level %d.", vec->level);
    return NULL;
  }

  //algorithm
  imc_rrb_t* new_root;
  if(imc_vector_concrete_full(vec)==1) {
    puts("vec is full.\n");
    //New root with vec as first child
    new_root = imc_vector_concrete_new_root(vec);
    //Create the path to add data
    new_root->children.subtrees[1] = imc_vector_concrete_create();
    vec = new_root->children.subtrees[1];
    vec->level = new_root->level - 1;
    vec->element_count = 1;
    while(vec->level != 1) {
      vec->children.subtrees[0] = imc_vector_concrete_create();
      vec->children.subtrees[0]->level = vec->level - 1;
      vec = vec->children.subtrees[0];
      vec->element_count = 1;
    }
    vec->children.subtrees[0] = imc_vector_concrete_create_leaf();
    vec->children.subtrees[0]->children.data[0] = data;
    new_root->element_count += 1;
  } else {
    puts("vec is not full.\n");

    new_root = imc_vector_concrete_copy(vec); // TODO : should also update refs to children
    puts("made a new root copy\n");
    vec = new_root;
if(vec == NULL) puts("yo man\n");
    int sub_index = imc_vector_concrete_subindex(vec, new_root->element_count);
    printf("level : %d sub_index : %d\n", vec->level, sub_index);
    while(vec->level != 1) {
       sub_index = imc_vector_concrete_subindex(vec, new_root->element_count); // On veut ajouter à l'indice nb_element
       vec->children.subtrees[sub_index]->refs -= 1; //has been up by previous copy, we need to do -1
       vec->children.subtrees[sub_index] = imc_vector_concrete_copy(vec->children.subtrees[sub_index]); // This function updates refs to children
       vec = vec->children.subtrees[sub_index];
    }
    puts("created path to leaf.\n");
    vec->children.subtrees[sub_index] = imc_vector_concrete_copy_leaf(vec->children.subtrees[sub_index]); //This should copy the data
    puts("DEBUG 1\n");
    vec = vec->children.subtrees[sub_index];
    puts("DEBUG 2\n");
    sub_index = imc_vector_concrete_subindex(vec, new_root->element_count); // On veut ajouter à l'indice nb_element
    puts("DEBUG 3\n");
    vec->children.data[sub_index] = data;
    puts("DEBUG 4\n");
  }
  return new_root;
}

imc_rrb_t* imc_vector_concrete_pop(imc_rrb_t* vec, imc_data_t** data) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return NULL;
}

int imc_vector_concrete_split(imc_rrb_t* vec_in, int index, imc_rrb_t** vec_out1, imc_rrb_t** vec_out2) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return -1;
}

imc_rrb_t* imc_vector_concrete_merge(imc_rrb_t* vec_front, imc_rrb_t* vec_tail) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return NULL;
}

/* user-side memory management */

int imc_vector_concrete_unref(imc_rrb_t* vec) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return -1;
}

void imc_vector_concrete_dump(imc_rrb_t* vec) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return;
}

/* utils */
void imc_vector_concrete_emit(imc_rrb_t* vec) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return;
}

/* return 1 if the vector is full */
int imc_vector_concrete_full(imc_rrb_t* vec) {
  if(imc_vector_concrete_balanced(vec) == 1) {
    return (vec->element_count == (pow(ARRAY_SIZE, vec->level))) ? 1 : 0;
  } else {
    while(vec->level > 1) {
      if(vec->meta[ARRAY_SIZE-1]==0) {
        return 0;
      }
    }
    vec = vec->children.subtrees[ARRAY_SIZE-1];
    return (vec->children.data[ARRAY_SIZE-1]!=NULL) ? 1 : 0;
  }
}

/* Add a new root to the top of a vector */
imc_rrb_t* imc_vector_concrete_new_root(imc_rrb_t* vec) {
  imc_rrb_t* new_root = NULL;
  new_root = imc_vector_concrete_create();
  new_root->children.subtrees[0] = vec;
  vec->refs+=1;
  new_root->element_count = vec->element_count;
  new_root->level = vec->level+1;
  return new_root;
}

/* return 1 if the vector is balanced */
int imc_vector_concrete_balanced(imc_rrb_t* vec) {
  /* /!\ Is that always true ? What if the rrb has been rebalanced ? /!\ */
  return (vec->meta==NULL) ? 1 : 0;
}

/* return the subindex, i.e. the subindex you may choose at your current
   level to go to the vector index */
int imc_vector_concrete_subindex(imc_rrb_t* vec, int index) {
  int level_index = 0;
  if(imc_vector_concrete_balanced(vec)==1) { //Si on est balanced, on sait tout de suite où aller
    level_index = (index >> (int)(log2((double)ARRAY_SIZE) * vec->level)) & (ARRAY_SIZE-1);
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

imc_rrb_t* imc_vector_concrete_copy_leaf(imc_rrb_t* vec) {
  puts("copying a leaf\n");
  imc_rrb_t* vec_copy = imc_vector_concrete_create();
  if(vec == NULL) puts("null mother fucker\n");
  vec_copy -> level = vec -> level;
  vec_copy -> element_count =  vec -> element_count;
  vec_copy -> meta = vec -> meta; // A changer en allouant un nouveau tableau meta
  if(vec -> children.data != NULL) {
    vec_copy -> children.data = malloc(sizeof(imc_data_t) * ARRAY_SIZE);
    for(int i = 0; i < ARRAY_SIZE ; i++) {
      vec_copy -> children.data[i] = vec -> children.data[i];
    }
  }
puts("leaf copy end\n");
  return vec_copy;
}

imc_rrb_t* imc_vector_concrete_copy(imc_rrb_t* vec) {
  imc_rrb_t* vec_copy = imc_vector_concrete_create();
  vec_copy -> level = vec -> level;
  vec_copy -> element_count =  vec -> element_count;
  vec_copy -> meta = vec -> meta; // A changer en allouant un nouveau tableau meta
  if(vec -> children.subtrees != NULL) {
    vec_copy -> children.subtrees = malloc(sizeof(imc_rrb_t) * ARRAY_SIZE);
    for(int i = 0; i < ARRAY_SIZE ; i++) {
      vec_copy -> children.subtrees[i] = vec -> children.subtrees[i];
      if(vec_copy -> children.subtrees[i] != NULL) {
        vec_copy -> children.subtrees[i] -> refs += 1;
      }
    }
  }

  return vec_copy;
}

imc_rrb_t* imc_vector_concrete_create_leaf() {
  imc_rrb_t* vec = malloc(sizeof(imc_rrb_t));

  if(vec == NULL){
      LOG(LOG_FATAL, "Allocation failure %s", strerror(errno));
      return NULL;
  }
  vec -> level = 0;
  vec -> refs = 1;
  vec -> element_count = 0;
  vec -> meta = NULL;
  vec -> children.data = malloc(sizeof(imc_data_t) * ARRAY_SIZE);

  if(vec -> children.data == NULL){
      LOG(1, "Allocation failure %s", strerror(errno));
      free(vec);
      return NULL;
  }

  return vec;
}
