#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"
#include "rrb_vect.h"

imc_vector_t* imc_vector_concrete_create() {
  imc_rrb_vector_t* rrb = malloc(sizeof(imc_rrb_vector));
  //inner allocations
  return (imc_vector_t*) rrb;
}

int imc_vector_concrete_size(imc_vector_t* vec) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return -1;
}

imc_vector_t* imc_vector_concrete_update(imc_vector_t* vec, int index, imc_data_t* data) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return NULL;
}

imc_data_t* imc_vector_concrete_lookup(imc_vector_t* vec, int index) {
  //TODO [9,18,21,-1,-1,-1] : Vérifier si ça ne cause pas de problèmes ! notamment dans la dernière cond.
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
  if(vec->is_unbalanced==0) { //Si on est balanced, on sait tout de suite où aller
    level_index = (index >> (log2((double)M) * vec->level)) & (M-1);
    if(vec->level == 1) { //Si la root est de niveau 1, on a trouvé l'élément
      return vec->data[level_index];
    }
  } else { //Si on est unbalanced, on doit chercher où aller
    while(vec->meta[level_index] <= index) {
      level_index++;
      //TODO : if meta level_ind <= -1 ?
    }
  }
  return lookup(vec->childs[level_index], index);
}

void imc_vector_concrete_emit(imc_vector_t* vec) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return;
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

  //

  return NULL;
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
