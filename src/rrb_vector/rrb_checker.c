#include <stdlib.h>
#include <stdio.h>
#include "../debug.h"
#include "./rrb_vect.h"
#include "./rrb_checker.h"

int imc_rrb_check_invariant(imc_rrb_t* vec){
  return count_elems(vec) == imc_rrb_size(vec); 
}

int count_elems(imc_rrb_t* vec){
  int i, count = 0;
  if(vec->floor == 0){
    imc_data_t** data = vec->node.data; 
    for(i = 0; i < ARRAY_SIZE-1; i++){
      if(data[i] != NULL) count++;
    }
    return count;
  }
  imc_rrb_t** next = vec->node.next;
  for(i = 0; i < ARRAY_SIZE-1; i++){
    if(next[i] != NULL) count += count_elems(next[i]);
  }
  return count;
}
