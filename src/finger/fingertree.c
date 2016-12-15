#include <stdio.h>
#include <stdlib.h>
#include "fingertree.h"

ft* ft_init() {
    ft* fingerTree = malloc(sizeof(ft));

    fingerTree->type = EMPTY_TYPE;
    fingerTree->ref_count = 1;

    return fingerTree;
}

/*
 * For the moment, only works to add 1 element */
ft* ft_add(void* data, ft* fgt) {
    ft* res:
    if (fgt->type == EMPTY_TYPE) {
        res = ft_init();
        create_single(res);
        create_node_d(res->true_ft->single);
        res->true_ft->single->true_node->data = data;
        return res;
    }
    else {
        printf("Error: Unsupported operation.\n");
        return NULL;
    }
}

/*
 * For the moment, only works for trees with 1 element */
void node_display(node* node) {
    if (node == NULL)
        printf("NULL");

    switch (node->type) {
        case DATA_TYPE:
            printf("%d", *((int*)node->true_node->data));
            break;
        case NODE_TYPE:
            printf("Node(");
            for (int i = 0; i < 3; i++)
                node_display(node->true_node->internal_node[i]);
            printf(")");
            break;
    }
}

/*
 * For the moment, only works for trees with 1 element */
void ft_display(ft* fgt) {
    switch (fgt->type) {
        case SINGLE_TYPE:
            printf("Single(");
            node_display(fgt->true_ft->single);
            printf(")");
            break;
        default:
            printf("Error: Unsupported\n");
    }
}


void create_single(ft* fgt){
    fgt->type = SINGLE_TYPE;
    fgt->true_ft = malloc(sizeof(true_ft_t));
    fgt->true_ft->single = malloc(sizeof(node));
    fgt->ref_count = 1;
}


void create_deep(ft* fgt){
  int i;
  fgt->type = DEEP_TYPE;
  fgt->true_ft->d = malloc(sizeof(true_ft_t));
  for(i=0;i<4;i++){
    fgt->true_ft->d->prefix[i]= malloc(sizeof(node));
    fgt->true_ft->d->suffix[i]= malloc(sizeof(node));
  }
  fgt->true_ft->d->deeper = malloc(sizeof(ft));
  fgt->ref_count = 1;
}

void create_data_node(node* n){
    n->type = DATA_TYPE;
    n->true_node = malloc(sizeof(true_node_t));
    n->ref_count = 1;
}

void checkInvariants() {
    // === Node Invariants===
    // Nodes are either NODE_TYPE or DATA_TYPE

    // A DATA_TYPE Node does not contain another Node

    // A NODE_TYPE Node does contain 2 or 3 Nodes

    // ===Finger tree Invariant===
    // Finger trees are either DEEP_TYPE, SINGLE_TYPE or EMPTY_TYPE

    // A SINGLE_TYPE Finger tree contains only one Node (and no infix).

    // Any Node in the infix and any Single at level 0 (the highest level) are DATA_TYPE.

    // Any Node in the infix and any Single at level n (the deeper level, the greater n) are NODE_TYPE^n(DATA_TYPE).

    // A DEEP_TYPE Finger tree contains infixes and a deeper Finger tree.

    // The infixes contain at least one elem each (i.e. at least 1 Node contains effective data)

    // The infixes are arrays of size four (i.e. contains 4 Node, even if unused)

    // In an infix containing n elements, these elements correspond to the n leftmost indexes of the array.

}
