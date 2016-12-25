#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "fingertree.h"

void checkSizeNode(node* n) {
    if (n->type == DATA_TYPE) {
        assert(n->size==1);
    } else {
        int predictedSize = 0;
        for (int i = 0; i < 3; i++) {
            if (n->true_node->internal_node[i] != NULL) {
                checkSizeNode(n->true_node->internal_node[i]);
                predictedSize += n->true_node->internal_node[i]->size;
            }
            else
                break;
        }
        
        assert(predictedSize == n->size);
    }
}

void checkSizeAffix(affix* a) {
    int predictedSize = 0;
    
    for (int i = 0; i < 4; i++) {
        if (a->nodes[i] != NULL) {
            checkSizeNode(a->nodes[i]);
            predictedSize += a->nodes[i]->size;
        }
        else
            break;
    }
    
    assert(predictedSize == a->size);
}

void checkSizeFingerTree(ft* fgt) {
    if (fgt->type == EMPTY_TYPE) {
        assert(fgt->size==0);
    }
    else if (fgt->type == SINGLE_TYPE) {
        node* n = fgt->true_ft->single;
        checkSizeNode(n);
        assert(fgt->size==n->size);
    }
    else if (fgt->type == DEEP_TYPE) {
        // Prefix
        affix* prefix = fgt->true_ft->d->prefix;
        checkSizeAffix(prefix);
        
        // Deeper
        ft* deeper = fgt->true_ft->d->deeper;
        checkSizeFingerTree(deeper);
        
        // Suffix
        affix* suffix = fgt->true_ft->d->suffix;
        checkSizeAffix(suffix);
        
        // Finger tree itself
        assert(fgt->size == prefix->size + deeper->size + suffix->size);
    }
}

void checkInvariants(ft* fgt) {
    // === Node Invariants===
    // Nodes are either NODE_TYPE or DATA_TYPE

    // A DATA_TYPE Node does not contain another Node

    // A NODE_TYPE Node does contain 2 or 3 Nodes

    // ===Finger tree Invariant===
    // Finger trees are either DEEP_TYPE, SINGLE_TYPE or EMPTY_TYPE

    // A SINGLE_TYPE Finger tree contains only one Node (and no affix).

    // Any Node in the affix and any Single at level 0 (the highest level) are
    // DATA_TYPE.

    // Any Node in the affix and any Single at level n (the deeper level, the
    // greater n) are NODE_TYPE^n(DATA_TYPE).

    // A DEEP_TYPE Finger tree contains affixes and a deeper Finger tree.

    // The affixes contain at least one elem each (i.e. at least 1 Node contains
    // effective data)

    // The affixes are arrays of size four (i.e. contains 4 Node, even if
    // unused)

    // In an affix containing n elements, these elements correspond to the n
    // leftmost indexes of the array.

    // ===ref_count Invariants===
    // TODO

    // ===size Invariants===
    // The size of a DATA_TYPE node should be 1.
    // The size of a NODE_TYPE node should be the number of DATA_NODE it contains
    // The size of an affix should be the sum of the size of its nodes
    // The size of a finger tree should be the sum of its components.
    checkSizeFingerTree(fgt);
}
