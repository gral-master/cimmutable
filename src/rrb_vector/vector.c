#include "../../include/vector.h"
#include "./rrb_vect.h"

//TODO check invariant and/or pre/post conditions
imc_vector_t* imc_vector_create() {

    imc_rrb_t* rrb = imc_rrb_create();

    return (imc_vector_t*)rrb;
}

//TODO check invariant and/or pre/post conditions
int imc_vector_size(imc_vector_t* vec) {
    return imc_rrb_size((imc_rrb_t*)vec);
}
