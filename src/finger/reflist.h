#ifndef reflist_h
#define reflist_h

#include "fingertree.h"

typedef struct reflist_t {
    node* elem;
    int ref;
    struct reflist_t * next;
} reflist;

typedef struct refdeep_t {
    ft* elem;
    int ref;
} refdeep;

reflist* rl_of_ft(ft* fgt);
void inter_verify(reflist* rl_fgt, reflist* rl_res);

#endif
