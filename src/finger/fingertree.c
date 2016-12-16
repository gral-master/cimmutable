#include <stdio.h>
#include <stdlib.h>
#include "fingertree.h"

ft* create_empty() {
    ft* fgt = malloc(sizeof(ft));
    fgt->type = EMPTY_TYPE;
    fgt->ref_count = 1;
    fgt->size = 0;
    return fgt;
}

ft* create_single(){
    ft* fgt = malloc(sizeof(ft));
    fgt->type = SINGLE_TYPE;
    fgt->true_ft = malloc(sizeof(true_ft_t));
    fgt->ref_count = 1;
    fgt->size = 1;
    return fgt;
}


ft* create_deep(){
    ft* fgt = malloc(sizeof(ft));
    int i;
    fgt->type = DEEP_TYPE;
    fgt->true_ft = malloc(sizeof(true_ft_t));
    fgt->true_ft->d = malloc(sizeof(deep));
    for(i=0;i<4;i++){
        fgt->true_ft->d->prefix[i]= NULL;
        fgt->true_ft->d->suffix[i]= NULL;
    }

    fgt->true_ft->d->deeper = create_empty();
    fgt->ref_count = 1;

    fgt->size = 0; // The caller of this function should put the right size.

    return fgt;
}

node* create_data_node(){
    node* n = malloc(sizeof(node));
    n->type = DATA_TYPE;
    n->true_node = malloc(sizeof(true_node_t));
    n->ref_count = 1;
    n->size = 1;
    return n;
}

/*
* For the moment, only works to add 1 element */
ft* ft_add(void* data, ft* fgt) {
    ft* res;
    if (fgt->type == EMPTY_TYPE) {
        res = create_single();
        res->true_ft->single = create_data_node();
        res->true_ft->single->true_node->data = data;
        res->size = 1;
        return res;
    }
    else {
        if (fgt->type == SINGLE_TYPE) {
            res = create_deep();
            fgt->true_ft->single->ref_count++;
            res->true_ft->d->prefix[0] = fgt->true_ft->single;
            res->true_ft->d->suffix[0] = create_data_node();
            res->true_ft->d->suffix[0]->true_node->data = data;
            res->size = 2;
            return res;
        }
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
        default:
        printf("_");
    }
}

/*
* For the moment, only works for trees with 1 element */
void ft_display(ft* fgt) {
    switch (fgt->type) {
        case EMPTY_TYPE:
        printf("EMPTY");
        break;
        case SINGLE_TYPE:
        printf("Single(");
        node_display(fgt->true_ft->single);
        printf(")");
        break;
        case DEEP_TYPE:
        printf("Deep([");

        // Prefix
        for (int i = 0; i < 4; i++)
            if (fgt->true_ft->d->prefix[i] != NULL)
                node_display(fgt->true_ft->d->prefix[i]);
        printf("],");
        // Deeper
        ft_display(fgt->true_ft->d->deeper);
        // Suffix
        printf(",[");
        for (int i = 0; i < 4; i++)
            if (fgt->true_ft->d->suffix[i] != NULL)
                node_display(fgt->true_ft->d->suffix[i]);
        printf("])");
        break;
        default:
        printf("Error: Unsupported\n");
    }
}

void checkInvariants() {
    // === Node Invariants===
    // Nodes are either NODE_TYPE or DATA_TYPE

    // A DATA_TYPE Node does not contain another Node

    // A NODE_TYPE Node does contain 2 or 3 Nodes

    // ===Finger tree Invariant===
    // Finger trees are either DEEP_TYPE, SINGLE_TYPE or EMPTY_TYPE

    // A SINGLE_TYPE Finger tree contains only one Node (and no infix).

    // Any Node in the infix and any Single at level 0 (the highest level) are
    // DATA_TYPE.

    // Any Node in the infix and any Single at level n (the deeper level, the
    // greater n) are NODE_TYPE^n(DATA_TYPE).

    // A DEEP_TYPE Finger tree contains infixes and a deeper Finger tree.

    // The infixes contain at least one elem each (i.e. at least 1 Node contains
    // effective data)

    // The infixes are arrays of size four (i.e. contains 4 Node, even if
    // unused)

    // In an infix containing n elements, these elements correspond to the n
    // leftmost indexes of the array.

    // ===ref_count Invariants===

    // ===size Invariants===

}
