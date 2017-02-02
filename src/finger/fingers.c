#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tools.h"
#include "fingers.h"

#define NODE_MAX_SIZE 4

/**
 * Return blank finger node with ref counter properly set
 */
fingernode_t* make_fingernode(int arity, node_type_t type) {
    finger_debug("make_fingernode\n");
    fingernode_t* res = malloc(sizeof(fingernode_t));
    res->ref_counter = 1;
    res->arity = arity;
    res->node_type = type;
    switch (type) {
    case DATA_NODE:
        res->content.data = malloc(arity * sizeof(finger_data_t*));
        break;
    case TREE_NODE:
        res->content.children = malloc(arity * sizeof(fingernode_t*));
        break;
    default:
        break;
    }
    return res;
}

/**
 * Return a comprehensive copy of node
 */
fingernode_t* copy_node(fingernode_t* node){
    fingernode_t* res = make_fingernode(node->arity, node->node_type);
    res->lookup_idx = node->lookup_idx;
    switch (res->node_type) {
    case DATA_NODE:
        memcpy(res->content.data, node->content.data, node->arity * sizeof(finger_data_t*));
        break;
    case TREE_NODE:
        memcpy(res->content.children, node->content.children, node->arity * sizeof(fingernode_t*));
        increment_children_refs(res);
        break;
    default:
        break;
    }
    return res;
}

/**
 * Update a fingernode's lookup index
 */
void update_lookup_idx(fingernode_t* node) {
    switch (node->node_type) {
    case TREE_NODE:
        node->lookup_idx = 0;
        for (int i=0; i<node->arity; i++) {
            node->lookup_idx += node->content.children[i]->arity;
        }
        break;
    case DATA_NODE:
        node->lookup_idx = node->arity;
        break;
    default:
        break;
    }
}

/**
 * Increment a node's children's refs
 */
void increment_children_refs(fingernode_t* node) {
    if (node->node_type == TREE_NODE) {
        for (int i=0; i<node->arity; i++) {
            node->content.children[i]->ref_counter++;
        }
    }
}

/**
 * Split a fingernode into 2 fingernodes, according to leftcount
 *   - Return the left side of the split
 *   - Put the pointer to the right side in rem
 */
fingernode_t* split_fingernode_content(int leftcount, fingernode_t* originals, fingernode_t** rem) {
    finger_debug("split_fingernode_content\n");
    int rightcount = originals->arity - leftcount;
    fingernode_t* res = make_fingernode(leftcount, originals->node_type);
    fingernode_t* remainder = make_fingernode(rightcount, originals->node_type);
    switch (originals->node_type) {
    case TREE_NODE:
        memcpy(res->content.children, originals->content.children, leftcount * sizeof(fingernode_t*));
        memcpy(remainder->content.children, originals->content.children + leftcount, rightcount * sizeof(fingernode_t*));
        increment_children_refs(res);
        increment_children_refs(remainder);
        break;
    case DATA_NODE:
        memcpy(res->content.data, originals->content.data, leftcount * sizeof(finger_data_t*));
        memcpy(remainder->content.data, originals->content.data + leftcount, rightcount * sizeof(finger_data_t*));
        break;
    }
    update_lookup_idx(res);
    update_lookup_idx(remainder);
    *rem = remainder;
    return res;
}

/**
 * Copy fingernode node, with a single element removed from the tail
 * of its content.
 */
fingernode_t* copy_remove_tail(fingernode_t* node) {
    finger_debug("copy_remove_tail\n");
    fingernode_t* res = make_fingernode(node->arity - 1, node->node_type);
    int size = node->arity * node->node_type == TREE_NODE ? sizeof(fingernode_t*) : sizeof(finger_data_t*);
    switch (node->node_type) {
    case TREE_NODE:
        memcpy(res->content.children, node->content.children, size * res->arity);
        break;
    case DATA_NODE:
        memcpy(res->content.data, node->content.data, size * res->arity);
        break;
    }
    update_lookup_idx(res);
    increment_children_refs(res);
    return res;
}

/**
 * Build a tree node from original tree nodes and new node
 * Guaranteed to be called with legit params.
 * ASSERT: depth of new_node is equal to depth of each node in nodes
 */
fingernode_t* make_treenode_and_cpy(int node_count, fingernode_t* new_node, fingernode_t** old_nodes, side_t side) {
    finger_debug("make_treenode_and_cpy\n");
    fingernode_t* res = make_fingernode(node_count + 1, TREE_NODE);
    switch (side) {
    case FINGER_LEFT:
        res->content.children[0] = new_node;
        for (int i=1; i<node_count+1; i++) {
            res->content.children[i] = old_nodes[i-1];
            res->content.children[i]->ref_counter++;
        }
        break;
    case FINGER_RIGHT:
        for (int i=0; i<node_count; i++) {
            res->content.children[i] = old_nodes[i];
            res->content.children[i]->ref_counter++;
        }
        res->content.children[node_count] = new_node;
        break;
    default:
        break;
    }
    res->lookup_idx = 0;
    for (int i=0; i<node_count+1; i++) {
        res->lookup_idx += res->content.children[i]->lookup_idx;
    }
    return res;
}

/**
 * Build a data node from original data nodes and new value
 */
fingernode_t* make_datanode_and_cpy(int data_count, finger_data_t* new_data, finger_data_t** old_data, side_t side) {
    finger_debug("make_datanode_and_cpy\n");
    fingernode_t* res = make_fingernode(data_count + 1, DATA_NODE);
    switch (side) {
    case FINGER_LEFT:
        res->content.data[0] = new_data;
        memcpy(res->content.data + 1, old_data, data_count * sizeof(finger_data_t*));
        break;
    case FINGER_RIGHT:
        memcpy(res->content.data, old_data, data_count * sizeof(finger_data_t*));
        res->content.data[data_count] = new_data;
        break;
    default:
        break;
    }
    res->lookup_idx = res->arity;
    return res;
}

/**
 * Split a treenode according to the new node and side to insert into
 * Returns the new node at current level
 * Puts into to_append the fingernode to append below
 */
fingernode_t* split_append_treenode (fingernode_t* new_node, fingernode_t* old_nodes, fingernode_t** to_append, side_t side) {
    finger_debug("split_append_treenode\n");
    fingernode_t* res_node = make_fingernode(2, TREE_NODE);
    fingernode_t* append_child = make_fingernode(3, old_nodes->node_type);
    fingernode_t* append = make_fingernode(1, TREE_NODE);
    switch (side) {
    case FINGER_LEFT:
        res_node->content.children[0] = new_node;
        res_node->content.children[1] = old_nodes->content.children[0];
        memcpy(append_child->content.children[0], old_nodes->content.children + 1, 3 * sizeof(fingernode_t*));
        break;
    case FINGER_RIGHT:
        res_node->content.children[0] = old_nodes->content.children[old_nodes->arity-1];
        res_node->content.children[1] = new_node;
        memcpy(append_child->content.children, old_nodes->content.children, 3 * sizeof(fingernode_t*));
        break;
    default:
        break;
    }
    append->content.children[0] = append_child;

    // Index updates
    update_lookup_idx(res_node);
    update_lookup_idx(append_child);
    update_lookup_idx(append);

    *to_append = append;
    return res_node;
}

/**
 * Split a datanode according to the new value and side to insert into
 * Returns the new node at current level
 * Puts into to_append the fingernode to append below
 */
fingernode_t* split_append_datanode (finger_data_t* new_value, fingernode_t* old_values, fingernode_t** to_append, side_t side) {
    finger_debug("split_append_datanode\n");
    fingernode_t* res_node = make_fingernode(2, DATA_NODE);
    fingernode_t* append_child = make_fingernode(3, DATA_NODE);
    fingernode_t* append = make_fingernode(1, TREE_NODE);
    switch (side) {
    case FINGER_LEFT:
        res_node->content.data[0] = new_value;
        res_node->content.data[1] = old_values->content.data[0];
        memcpy(append_child->content.data, old_values->content.data + 1, 3 * sizeof(finger_data_t*));
        break;
    case FINGER_RIGHT:
        res_node->content.data[0] = old_values->content.data[old_values->arity-1];
        res_node->content.data[1] = new_value;
        memcpy(append_child->content.data, old_values->content.data, 3 * sizeof(finger_data_t*));
        break;
    default:
        break;
    }
    append->content.children[0] = append_child;

    // Index updates
    update_lookup_idx(res_node);
    update_lookup_idx(append_child);
    update_lookup_idx(append);

    *to_append = append;
    return res_node;
}

/**
 * Return blank deep node with ref counter properly set
 */
deep_t* make_deep() {
    finger_debug("make_deep\n");
    deep_t* res = malloc(sizeof(deep_t));
    res->ref_counter = 1;
    res->tag = 0;
    return res;
}

/**
 * Build an empty node. This can be simplified to a global, indelible empty node variable
 * since we can reference the same one multiple times without impacting the tree's semantics.
 */
deep_t* make_empty_node() {
    finger_debug("make_empty_node\n");
    deep_t* res = make_deep();
    res->deep_type = EMPTY_NODE;
    return res;
}

/**
 * Build a single node from the fingernode it is supposed to contain
 */
deep_t* make_single_node(fingernode_t* finger) {
    finger_debug("make_single_node\n");
    deep_t* res = make_deep();
    res->deep_type = SINGLE_NODE;
    res->content.single = finger;
    return res;
}

/**
 * Build a deep node, which is a dorsal node of the finger tree
 */
deep_t* make_deep_node(fingernode_t* left, deep_t* deeper, fingernode_t* right) {
    finger_debug("make_deep_node\n");
    deep_t* res = make_deep();
    res->deep_type = DEEP_NODE;
    res->left = left;
    res->content.deeper = deeper;
    res->right = right;
    return res;
}

/**
 * Destroy a fingernode, no recursion
 */
void destroy_fingernode(fingernode_t* node) {
    finger_debug("destroy_fingernode\n");
    switch (node->node_type) {
    case TREE_NODE:
        free(node->content.children);
        break;
    case DATA_NODE:
        free(node->content.data);
        break;
    default:
        break;
    }
    free(node);
}

/**
 * Destroy a deep, no recursion
 */
void destroy_deep(deep_t* deep) {
    finger_debug("destroy_deep\n");
    free(deep);
}

/**
 * Recursively update reference counts in node to have one less reference
 */
int unref_fingernode(fingernode_t* node) {
    finger_debug("unref_fingernode\n");
    node->ref_counter--;
    if (node->ref_counter) { // i.e. ref_count != 0
        return 1;
    }
    // recursive unref
    if (node->node_type == TREE_NODE) {
        fingernode_t** children = node->content.children;
        for (int i=0; i<node->arity; i++) {
            unref_fingernode(children[i]);
        }
    }
    destroy_fingernode(node);
    return 1;
}

/**
 * Recursively update reference counts in deep to have one less reference
 */
int unref_deep(deep_t* deep) {
    finger_debug("unref_deep\n");
    deep->ref_counter--;
    if (deep->ref_counter) {
        return 0;
    }
    switch (deep->deep_type) {
    case DEEP_NODE:
        unref_fingernode(deep->left);
        unref_deep(deep->content.deeper);
        unref_fingernode(deep->right);
    case SINGLE_NODE:
        unref_fingernode(deep->content.single);
        break;
    case EMPTY_NODE:
    default:
        break;
    }
    destroy_deep(deep);
    return 1;
}

/**
 * Using the display function for the finger data type, recursively dump node
 */
void dump_finger(fingernode_t* node, int span, void (*display)(finger_data_t**, int)) {
    fprintf(stdout, "%*s", span, "");
    fprintf(stdout, "finger {\n");
    switch (node->node_type) {
    case TREE_NODE:
        for (int i=0; i<node->arity; i++) {
            dump_finger(node->content.children[i], span + 2, display);
        }
        break;
    case DATA_NODE:
        fprintf(stdout, "%*s", span + 2, "");
        display(node->content.data, node->arity);
    default:
        break;
    }
    fprintf(stdout, "%*s}\n", span, "");
}

/**
 * Using the display function for the finger data type, recursively dump deep
 */
void dump_deep(deep_t* deep, int span, void (*display)(finger_data_t**, int)) {
    fprintf(stdout, "%*s", span, "");
    fprintf(stdout, "deep {\n");
    switch (deep->deep_type) {
    case DEEP_NODE:
        fprintf(stdout, "%*s", span + 2, "");
        fprintf(stdout, "left: ");
        dump_finger(deep->left, span + 4, display);
        fprintf(stdout, "%*s", span + 2, "");
        fprintf(stdout, "right: ");
        dump_finger(deep->right, span + 4, display);
        fprintf(stdout, "%*s", span + 2, "");
        fprintf(stdout, "deeper: ");
        dump_deep(deep->content.deeper, span + 4, display);
        break;
    case SINGLE_NODE:
        fprintf(stdout, "%*s", span + 2, "");
        fprintf(stdout, "single: ");
        dump_finger(deep->content.single, span + 2, display);
        break;
    case EMPTY_NODE:
        fprintf(stdout, "%*s", span + 2, "");
        fprintf(stdout, "empty!\n");
    default:
        break;
    }
    fprintf(stdout, "%*s", span, "");
    fprintf(stdout, "}\n");
}

/**
 * Given a finger node, prepend it in the tree rooted in deep
 */
deep_t* append_node(deep_t* deep, fingernode_t* node, side_t side) {
    finger_debug("append_node ");
    finger_args("deep %p node %p side %d", deep, node, side);
    switch(deep->deep_type) {
    case DEEP_NODE:
        finger_debug("\t\tdeep\n");
        break;
    case SINGLE_NODE:
        finger_debug("\t\tsingle\n");
        break;
    case EMPTY_NODE:
        finger_debug("\t\tempty\n");
    }

    deep_t* newdeep = make_deep();
    switch (deep->deep_type) {
    case DEEP_NODE:
        newdeep->deep_type = DEEP_NODE;
        break;
    case SINGLE_NODE:
        newdeep->deep_type = DEEP_NODE;
        break;
    case EMPTY_NODE:
        newdeep->deep_type = SINGLE_NODE;
        break;
    default:
        break;
    }

    if (deep->deep_type == DEEP_NODE) {
        finger_debug("deep\n");
        fingernode_t* mod_node = side == FINGER_LEFT ? deep->left : deep->right;
        if (mod_node->arity == NODE_MAX_SIZE) {
            fingernode_t* append;
            fingernode_t* new_mod_node = split_append_treenode(node, mod_node, &append, side);
            switch (side) {
            case FINGER_LEFT:
                newdeep->left = new_mod_node;
                newdeep->right = deep->right;
                if (newdeep->right) {
                    newdeep->right->ref_counter++;
                }
                break;
            case FINGER_RIGHT:
                newdeep->right = new_mod_node;
                newdeep->left = deep->left;
                if (newdeep->left) {
                    newdeep->left->ref_counter++;
                }
                break;
            default:
                break;
            }
            // Deeper recur
            newdeep->content.deeper = append_node(deep->content.deeper, append, side);
        } else {
            newdeep->content.deeper = deep->content.deeper;
            newdeep->content.deeper->ref_counter++;
            switch (side) {
            case FINGER_LEFT:
                newdeep->right = deep->right;
                newdeep->right->ref_counter++;
                newdeep->left = make_treenode_and_cpy(mod_node->arity, node, mod_node->content.children, side);
                break;
            case FINGER_RIGHT:
                newdeep->left = deep->left;
                newdeep->left->ref_counter++;
                newdeep->right = make_treenode_and_cpy(mod_node->arity, node, mod_node->content.children, side);
            default:
                break;
            }
        }
        return newdeep;
    }

    if (deep->deep_type == SINGLE_NODE) {
        finger_debug("single\n");
        fingernode_t* single = deep->content.single;
        if (single->arity == NODE_MAX_SIZE) {
            single->ref_counter++;
            switch (side) {
            case FINGER_LEFT:
                return make_deep_node(node, make_empty_node(), single);
            case FINGER_RIGHT:
                return make_deep_node(single, make_empty_node(), node);
            default:
                return NULL;
            }
        } else {
            return make_single_node(make_treenode_and_cpy(single->arity, node, single->content.children, side));
        }
    }
    if (deep->deep_type == EMPTY_NODE) {
        return make_single_node(node);
    }
    return NULL;
}

/**
 * Given a value, attempt to append it to the tree rooted in tree,
 * and append it recursively as a node if this is not possible
 * Will add to the front with FINGER_LEFT, to the back with FINGER_RIGHT
 */
deep_t* append(deep_t* tree, finger_data_t* value, side_t side) {
    finger_debug("prepend\n");
    deep_t* newdeep = make_deep();
    if (tree->deep_type == EMPTY_NODE) {
        newdeep->deep_type = SINGLE_NODE;
    } else {
        newdeep->deep_type = DEEP_NODE;
    }

    if (tree->deep_type == DEEP_NODE) {
        finger_debug("deep\n");

        fingernode_t* mod_node = side == FINGER_LEFT ? tree->left : tree->right;
        if (mod_node->arity == NODE_MAX_SIZE) {
            // Do the changes at the root node
            fingernode_t* append;
            fingernode_t* new_mod_node = split_append_datanode(value, mod_node, &append, side);
            switch (side) {
            case FINGER_LEFT:
                newdeep->left = new_mod_node;
                newdeep->right = tree->right;
                if (newdeep->right) {
                    newdeep->right->ref_counter++;
                }
                break;
            case FINGER_RIGHT:
                newdeep->right = new_mod_node;
                newdeep->left = tree->left;
                if (newdeep->left) {
                    newdeep->left->ref_counter++;
                }
                break;
            default:
                break;
            }
            // Deeper recur
            newdeep->content.deeper = append_node(tree->content.deeper, append, side);
        } else {
            newdeep->content.deeper = tree->content.deeper;
            newdeep->content.deeper->ref_counter++;
            switch (side) {
            case FINGER_LEFT:
                newdeep->right = tree->right;
                newdeep->right->ref_counter++;
                newdeep->left = make_datanode_and_cpy(mod_node->arity, value, mod_node->content.data, side);
                break;
            case FINGER_RIGHT:
                newdeep->left = tree->left;
                newdeep->left->ref_counter++;
                newdeep->right = make_datanode_and_cpy(mod_node->arity, value, mod_node->content.data, side);
            default:
                break;
            }
        }
        return newdeep;
    }

    fingernode_t* valuenode = make_fingernode(1, DATA_NODE);
    valuenode->content.data[0] = value;
    if (tree->deep_type == SINGLE_NODE) {
        finger_debug("single\n");
        fingernode_t* single = tree->content.single;
        if (single->arity >= NODE_MAX_SIZE) {
            single->ref_counter++;
            switch (side) {
            case FINGER_LEFT:
                return make_deep_node(valuenode, make_empty_node(), single);
            case FINGER_RIGHT:
                return make_deep_node(single, make_empty_node(), valuenode);
            default:
                return NULL;
            }
        } else {
            return make_single_node(make_datanode_and_cpy(single->arity, value, single->content.data, side));
        }
    }

    if (tree->deep_type == EMPTY_NODE) {
        finger_debug("empty\n");
        return make_single_node(valuenode);
    }
    return NULL;
}

/**
 * Given a finger tree and a reference to a pointer, do the recursive work for
 * balancing the finger tree after deletion of the last value
 * data is wrapped inside a fingernode for type issues
 */
deep_t* pop_deep(deep_t* tree, fingernode_t** data) {
    finger_debug("pop_deep\n");
    if (tree->deep_type == EMPTY_NODE) {
        *data = NULL;
        return NULL;
    } else if (tree->deep_type == SINGLE_NODE) {
        fingernode_t* single = tree->content.single;
        *data = tree->content.single->content.children[single->arity - 1];
        if (single->arity == 1) {
            return make_empty_node();
        } else {
            return make_single_node(copy_remove_tail(single));
        }
    } else if (tree->deep_type == DEEP_NODE) {
        fingernode_t* cur_node = tree->right;
        *data = cur_node->content.children[cur_node->arity - 1];
        deep_t* new_deep = NULL;
        if (cur_node->arity > 1) { // Just chop from the suffix
            fingernode_t* new_suffix = copy_remove_tail(cur_node);
            tree->left->ref_counter++;
            tree->content.deeper->ref_counter++;
            new_deep = make_deep_node(tree->left, tree->content.deeper, new_suffix);
        } else if (cur_node->arity == 1) {                 // We recursive boyz
            fingernode_t* promo_node;
            deep_t* pop_deeper = pop_deep(tree->content.deeper, &promo_node);
            if (!pop_deeper) {   // We're splitting the prefix/suffix and making a deep->empty
                if (tree->left->arity == 1) {
                    tree->left->ref_counter++;
                    new_deep = make_single_node(tree->left);
                } else {
                    fingernode_t* prefix;
                    fingernode_t* suffix;
                    prefix = split_fingernode_content(1, tree->left, &suffix);
                    new_deep = make_deep_node(prefix, make_empty_node(), suffix);
                }
            } else {               // Just append node to the prefix/suffix
                promo_node->ref_counter++;
                new_deep = make_deep_node(tree->left, pop_deeper, promo_node);
            }
        }
        return new_deep;
    } else {
        return NULL;
    }
}

/**
 * Given a finger tree and a reference to a pointer, store the last
 * value of the finger tree into the pointer, and return finger tree created by
 * removing that value from the end of the vector
 */
deep_t* pop(deep_t* tree, finger_data_t** data) {
    finger_debug("pop\n");
    if (tree->deep_type == EMPTY_NODE) {
        *data = NULL;
        return NULL;
    } else if (tree->deep_type == SINGLE_NODE) {
        fingernode_t* single = tree->content.single;
        *data = tree->content.single->content.data[single->arity - 1];
        if (single->arity == 1) {
            return make_empty_node();
        } else {
            return make_single_node(copy_remove_tail(single));
        }
    } else if (tree->deep_type == DEEP_NODE) {
        fingernode_t* cur_node = tree->right;
        *data = cur_node->content.data[cur_node->arity - 1];
        deep_t* new_deep = NULL;
        if (cur_node->arity > 1) { // Just chop from the suffix
            fingernode_t* new_suffix = copy_remove_tail(cur_node);
            tree->left->ref_counter++;
            tree->content.deeper->ref_counter++;
            new_deep = make_deep_node(tree->left, tree->content.deeper, new_suffix);
        } else if (cur_node->arity == 1) {                 // We recursive boyz
            fingernode_t* promo_node;
            deep_t* pop_deeper = pop_deep(tree->content.deeper, &promo_node);
            if (!pop_deeper) {                // We're splitting the prefix/suffix and making a deep->empty
                if (tree->left->arity == 1) {
                    tree->left->ref_counter++;
                    new_deep = make_single_node(tree->left);
                } else {
                    fingernode_t* prefix;
                    fingernode_t* suffix;
                    prefix = split_fingernode_content(1, tree->left, &suffix);
                    new_deep = make_deep_node(prefix, make_empty_node(), suffix);
                }
            } else {               // We're promoting the node lower node, so suffix should actually be empty rn
                promo_node->ref_counter++;
                new_deep = make_deep_node(tree->left, pop_deeper, promo_node);
            }
        }
        return new_deep;
    } else {
        return NULL;
    }
}

/**
 * Get a vector's number of elements
 */
int vector_size(deep_t* tree) {
    finger_debug("size\n");
    deep_t* deep_cur = tree;
    int size = 0;
    while (deep_cur->deep_type == DEEP_NODE) {
        size += deep_cur->left->lookup_idx;
        size += deep_cur->right->lookup_idx;
        deep_cur = deep_cur->content.deeper;
    }
    if (deep_cur->deep_type == SINGLE_NODE) {
        size += deep_cur->content.single->lookup_idx;
    }
    return size;
}

/**
 * Given an index and a starting index, find the corresponding value in a fingernode
 */
finger_data_t* lookup_fingernodes(fingernode_t* node, int idx, int idx_cur) {
    finger_debug("lookup_fingernodes\n");
    if (node->node_type == DATA_NODE) {
        return node->content.data[idx_cur-idx];
    }
    int node_idx = idx_cur;
    for (int i=0; i<node->arity; i++) {
        int child_idx = node->content.children[i]->lookup_idx;
        if (idx_cur+child_idx >= idx) {
            return lookup_fingernodes(node->content.children[i], idx, node_idx);
        }
        idx_cur += child_idx;
    }
    fprintf(stderr, "Index out of bounds");
    exit(1);
}

/**
 * Lookup the value associated to index idx
 * This does NOT modify the tree
 */
finger_data_t* lookup(deep_t* tree, int idx) {
    finger_debug("lookup\n");
    int i=0;
    fingernode_t* finger_cur;
    deep_list_t* stack = NULL;
    while (tree->deep_type == DEEP_NODE) {
        finger_debug("deep node\n");
        finger_cur = tree->left;
        if (i+finger_cur->lookup_idx >= idx) {
            return lookup_fingernodes(finger_cur, i, idx);
        }
        list_push(tree, &stack);
        i += finger_cur->lookup_idx;
        tree = tree->content.deeper;
    }
    if (tree->deep_type == SINGLE_NODE) {
        finger_debug("single node\n");
        finger_cur = tree->content.single;
        if (i+finger_cur->lookup_idx >= idx) {
            return lookup_fingernodes(finger_cur, i, idx);
        }
        i += finger_cur->lookup_idx;
    }
    deep_t* deep_cur;
    while (!list_is_empty(stack)) {
        finger_debug("back up\n");
        deep_cur = list_pop(&stack);
        finger_cur = deep_cur->right;
        if (i+finger_cur->lookup_idx >= idx) {
            return lookup_fingernodes(finger_cur, i, idx);
        }
        i += finger_cur->lookup_idx;
    }
    fprintf(stderr, "Index out of bounds");
    exit(1);
}

/**
 * Given a starting index and index, recreate a finger with the correct value updated
 */
fingernode_t* update_fingernode(fingernode_t* node, int cur_idx, int idx, finger_data_t* new_value) {
    finger_debug("update_fingernode\n");
    fingernode_t* res = copy_node(node);
    if (node->node_type == DATA_NODE) {
        res->content.data[idx-cur_idx] = new_value;
    }
    else {
        for (int i=0; i<node->arity; i++) {
            int child_idx = node->content.children[i]->lookup_idx;
            if (cur_idx+child_idx >= idx) {
                res->content.children[i] = update_fingernode(res->content.children[i], cur_idx, idx, new_value);
                for (int j=0; j<node->arity; j++) {
                    if (j != i) {
                        res->content.children[i]->ref_counter++;
                    }
                }
                break;
            }
            cur_idx += child_idx;
        }
    }
    return res;
}

/**
 * Given a depth, starting index and index, recreate a tree updated with the new value
 * Either we have arrived at depth 0, in which case we return the node
 * Or we have a non-zero depth, in which case we make a recursive call on the next dorsal node
 */
deep_t* update_up_to_depth (deep_t* tree, int depth, int cur_idx, int idx, side_t side, finger_data_t* new_value) {
    finger_debug("update_to_depth\n");
    if (depth == 0) {
        if (tree->deep_type == SINGLE_NODE) {
            return make_single_node(update_fingernode(tree->content.single, cur_idx, idx, new_value));
        }
        switch (side) {
        case FINGER_LEFT:
            tree->right->ref_counter++;
            tree->content.deeper->ref_counter++;
            return make_deep_node(update_fingernode(tree->left, cur_idx, idx, new_value), tree->content.deeper, tree->right);
        case FINGER_RIGHT:
            tree->left->ref_counter++;
            tree->content.deeper->ref_counter++;
            return make_deep_node(tree->left, tree->content.deeper, update_fingernode(tree->right, cur_idx, idx, new_value));
        default:
            return NULL;
        }
    }
    deep_t* deeper; // Depth not yet reached, tree HAS to be a DEEP_NODE
    switch (tree->deep_type) {
    case DEEP_NODE:
        deeper = update_up_to_depth(tree->content.deeper, depth-1, cur_idx, idx, side, new_value);
        tree->left->ref_counter++;
        tree->right->ref_counter++;
        return make_deep_node(tree->left, deeper, tree->right);
    case SINGLE_NODE: // We've made a mistake if we're here or below.
    case EMPTY_NODE:
    default:
        fprintf(stderr, "Update error - unexpected end of deep chain\n");
        return NULL;
    }
}

/**
 * Update tree val of index idx to value new_value
 * 3 STEPS:
 *   - Discover the depth of the fingernode containing the index value
 *   - Create a new tree through the dorsal up to this depth
 *   - Update the fingernode at this depth
 */
deep_t* update_deep(deep_t* tree, int idx, finger_data_t* new_value) {
    finger_debug("update_deep\n");
    // Get the dorsal position of the value, current index at position and side of the value in the dorsal node
    deep_list_t* stack = NULL;
    deep_t* deep_cur = tree;
    fingernode_t* finger_cur;
    int cur_idx = 0;
    side_t side = FINGER_LEFT;
    while (deep_cur->deep_type == DEEP_NODE) {
        finger_cur = deep_cur->left;
        if (cur_idx+finger_cur->lookup_idx >= idx) {
            goto found_deep_index;
        }
        cur_idx += finger_cur->lookup_idx;
        list_push(deep_cur, &stack);
        deep_cur = deep_cur->content.deeper;
    }
    if (deep_cur->deep_type == SINGLE_NODE) {
        finger_cur = deep_cur->content.single;
        if (cur_idx+finger_cur->lookup_idx >= idx) {
            goto found_deep_index;
        }
        cur_idx += finger_cur->lookup_idx;
    }
    side = FINGER_RIGHT;
    while (!list_is_empty(stack)) {
        deep_cur = list_pop(&stack);
        finger_cur = deep_cur->right;
        if (cur_idx+finger_cur->lookup_idx >= idx) { // finger_cur == NULL
            goto found_deep_index;
        }
        cur_idx += finger_cur->lookup_idx;
    }

 found_deep_index: ;
    // Found. Let's destroy the stack and update up to the depth we found
    int depth = list_size(stack);
    list_destroy(stack);
    return update_up_to_depth(tree, depth, cur_idx, idx, side, new_value);
}

deep_t* append_node_to_depth(deep_t* tree, fingernode_t* finger, side_t side) {
    if (tree->deep_type == DEEP_NODE) {
        int f_depth = finger_depth(finger);
        int d_depth = finger_depth(finger->node_type == FINGER_LEFT ? tree->left : tree->right);
        if (f_depth != d_depth) {
            deep_t* deeper = append_node_to_depth(tree->content.deeper, finger, side);
            return make_deep_node(tree->left, deeper, tree->right);
        } else {
            return append_node(tree, finger, side);
        }
    }
    if (tree->deep_type == SINGLE_NODE) {
        return NULL;
    }
    if (tree->deep_type == EMPTY_NODE) {
        return make_single_node(finger);
    }
    return NULL;
}

/**
 * Recursive merging of 2 finger trees, using
 * middle as a stack for nodes left
 */
deep_t* merge_with_middle(deep_t* left, finger_deque_t* middle, deep_t* right) {
    finger_debug("merge_with_middle\n");
    if (left->deep_type == DEEP_NODE) {
        if (deque_size(middle) == 0) {
            return right;
        } else {
            fingernode_t* x = deque_pop_first(middle);
            deep_t* new = merge_with_middle(make_empty_node(), middle, right);
            return append_node_to_depth(right, x, FINGER_LEFT);
        }
    } else if (left->deep_type == SINGLE_NODE) {
        fingernode_t* y = left->content.single;
        deep_t* new = merge_with_middle(make_empty_node(), middle, right);
        return append_node_to_depth(new, y, FINGER_LEFT);
    } else if (right->deep_type == EMPTY_NODE) {
        if (deque_size(middle) == 0) {
            return left;
        } else {
            fingernode_t* last = deque_pop_last(middle);
            deep_t* new = merge_with_middle(left, middle, make_empty_node());
            return append_node_to_depth(new, last, FINGER_RIGHT);
        }
    } else if (right->deep_type == SINGLE_NODE) {
        fingernode_t* y = left->content.single;
        deep_t* new = merge_with_middle(left, middle, make_empty_node());
        return append_node_to_depth(new, y, FINGER_LEFT);
    } else {                                   // This is the crazy recursive case
        deep_t* deeper = NULL;
    }
    return NULL;
}

/**
 * Merge 2 trees together
 */
/*
deep_t* merge(deep_t* left, deep_t* right) {
    finger_debug("merge\n");
    return merge_with_middle(left, deque_make(), right);
}

int main(void) {
    deep_t* tree = make_empty_node();

    int size = 1000;

    fprintf(stdout, "Append\n");
    for (int i = 0; i < size; i++) {
        fprintf(stderr, "%d, ", i);
        int* data = malloc(sizeof(int));
        *data = i;
        tree = append(tree, data, FINGER_RIGHT);
        if (!tree)
            fprintf(stderr, "what the shit\n");
    }
    fprintf(stdout, "Size: %d (should be %d)\n", vector_size(tree), size);

    fprintf(stdout, "Lookup\n");
    fprintf(stdout, "%d\n", *lookup(tree, 2));

    dump_deep(tree, 0, display);

    fprintf(stdout, "\nUpdate\n");
    int* upd = malloc(sizeof(int));
    *upd = 589420;
    tree = update_deep(tree, 0, upd);

    dump_deep(tree, 0, display);

    fprintf(stdout, "pop \n");
    int* pop_val;
    tree = pop(tree, &pop_val);
    fprintf(stdout, "Last value: %d\n", *pop_val);
    fprintf(stdout, "Remaining values:\n");
    dump_deep(tree, 0, display);
}
*/
