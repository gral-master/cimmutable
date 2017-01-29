#include "../../include/vector.h"
#include "./rrb_vect.h"
#include "./rrb_checker.h"
#include "../debug.h"

#define CHECK_INVARIANT(vec) imc_rrb_check_invariant((imc_rrb_t*)vec)

imc_vector_t* imc_vector_create() {
    
    imc_rrb_t* rrb = imc_rrb_create();
    if(debug_level <= 3){
        if(!CHECK_INVARIANT(rrb)){
            LOG(LOG_ERR, "imc_vector_create -- Invariant failure");
        }
    }
    return (imc_vector_t*)rrb;
}

//TODO check invariant and/or pre/post conditions
int imc_vector_size(imc_vector_t* vec) {
    if(debug_level <= 3){
        if(vec == NULL) LOG(LOG_ERR, "imc_vector_size -- Argument vec is null");
        if(!CHECK_INVARIANT(vec)) 
            LOG(LOG_ERR, "imc_vector_size -- Invariant failure");
    }
    return imc_rrb_size((imc_rrb_t*)vec);
}
