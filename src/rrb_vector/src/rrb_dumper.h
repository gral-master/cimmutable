#pragma once

#include "rrb_vector.h"
#include <stdio.h>

/** Dumps a RRB-Tree to a dotfile. Non working ? */
void rrb_dump(rrb_vector_t *rrb, char *file);

/** RRB-Tree Pretty Print. */
void rrb_pp(const rrb_vector_t *rrb);

/** RRB-Tree Pretty Print with Pointers. */
void rrb_ppp(const rrb_vector_t *rrb);

/** RRB-Tree Pretty Print with Pointers and Leafs. */
void rrb_ppp_leafs(const rrb_vector_t *rrb);
