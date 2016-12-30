#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "fingertree.h"
#include "reflist.h"

reflist* rl_add(node* n, int count, reflist* l) {
    reflist* new_rl = malloc(sizeof(reflist));
    
    new_rl->elem = n;
    new_rl->ref = count;
    new_rl->next = l;
    
    return new_rl;
}

reflist* rl_of_ft(ft* fgt) {
    if (fgt->type == EMPTY_TYPE) {
        return NULL;
    }
    else if (fgt->type == SINGLE_TYPE) {
        return rl_add(fgt->true_ft->single, fgt->true_ft->single->ref_count, NULL);
    }
    else {
        reflist *res = NULL;
        deep *d = fgt->true_ft->d;
        // Prefix
        for (int i = 0; i < 4; i++) {
            if (d->prefix->nodes[i] != NULL) {
                res = rl_add(d->prefix->nodes[i], d->prefix->nodes[i]->ref_count, res);
            }
            else
                break;
        }
        
        // Suffix
        for (int i = 0; i < 4; i++) {
            if (d->suffix->nodes[i] != NULL) {
                res = rl_add(d->suffix->nodes[i], d->suffix->nodes[i]->ref_count, res);
            }
            else
                break;
        }
        
        return res;
    }
}

void inter_verify(reflist* rl_fgt, reflist* rl_res) {
    reflist *tmp1, *tmp2;
    
    for (tmp1 = rl_fgt; tmp1 != NULL; tmp1 = tmp1->next) {
        for (tmp2 = rl_res; tmp2 != NULL; tmp2 = tmp2->next) {
            if (tmp1->elem == tmp2->elem) {
                // TODO
                // It should be == instead of <=
                // But ft_delete does not yet work when it is ==
	      /* puts(""); */
	      /* printf("tmp1 : %d, tmp2 : %d\n",tmp1->ref,tmp2->ref); */
	      assert(tmp1->ref + 1 == tmp2->ref);
                break;
            }
        }
    }
}
