#include <stdlib.h>
#include <stdio.h>
#include "../debug.h"
#include "./rrb_vect.h"
#include "./rrb_checker.h"

int imc_rrb_check_invariant(imc_rrb_t* vec){
    return (count_elems(vec) == imc_rrb_size(vec));
}

int count_elems(imc_rrb_t* vec){
    int i, count = 0;
    if(vec->floor == 0){
        imc_data_t** data = vec->node.data; 
        for(i = 0; i < ARRAY_SIZE; i++){
            if(data[i] != NULL) count++;
        }
        return count;
    }
    imc_rrb_t** next = vec->node.next;
    for(i = 0; i < ARRAY_SIZE; i++){
        if(next[i] != NULL) count += count_elems(next[i]);
    }
    return count;
}

int verif_balance(imc_rrb_t* vec){
    int i;
    if(imc_rrb_is_balanced(vec)){
        if(vec->floor > 0){
            int cond;            
            int res = 1;
            if(vec->node.next[0] != NULL) cond = 1;
            else cond = -1;
            for(i = 1; i < ARRAY_SIZE; i++){
                if(vec->node.next[i] == NULL && cond == 1){ 
                    cond = -1; 
                    continue;
                }
                if(vec->node.next[i] != NULL){ 
                    res = verif_balance(vec->node.next[i]);
                    if(cond == -1 || res == 0) return 0;
                }
            }
            return 1;
        }
        else{
            int cond = vec->node.next[0] == NULL;
            for(i = 1; i < ARRAY_SIZE; i++){
                if(cond && vec->node.next[i] != NULL) return 0;
                cond = vec->node.next[i] == NULL;
            }
            return 1;
        }
    }
    else{
        if(vec->floor > 0){
            int res = 1;
            int accu = 0;
            for(i = 0; i < ARRAY_SIZE; i++){
                if(vec->node.next[i] != NULL){
                    res = verif_balance(vec->node.next[i]);
                    if(res == 0) return 0;
                    accu += count_elems(vec->node.next[i]);
                    if(accu != vec->meta[i]) return 0;
                }
            }
            return 1;
        }
        else{
            int cond = vec->node.next[0] == NULL;
            for(i = 1; i < ARRAY_SIZE; i++){
                if(cond && vec->node.next[i] != NULL) return 0;
                cond = vec->node.next[i] == NULL;
            }
            return 1;
        }
    }       
}
