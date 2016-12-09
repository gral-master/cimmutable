#include "../../include/vector.h"
#include "rrb_vect.h"

//TODO check invariant and/or pre/post conditions
imc_vector_t* imc_vector_create(){

    imc_rrb_t* rrb = imc_vector_concrete_create();

    return (imc_vector_t*)rrb;
}
