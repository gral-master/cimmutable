#include "rrb_vector.h"

#define DEBUG 0

/** Creates an empty array of nodes into rrb. */
void make_nodes(rrb_t *rrb) {
    debug_print("make_nodes, beginning\n");
    rrb->leafs = false;
    rrb->nodes.child = calloc(32, sizeof *rrb->nodes.child);
    debug_print("make_nodes, end\n");
}

/** Creates an empty array of leafs into rrb. */
void make_datas(rrb_t *rrb) {
    debug_print("make_datas, beginning\n");
    rrb->leafs = true;
    rrb->nodes.leaf = calloc(32, sizeof *rrb->nodes.leaf);
    debug_print("make_datas, end\n");
}

/** Creates an empty RRB-Tree. Top is used in order to determine if it contains
* leafs or RRB-Nodes. */
rrb_t *create(bool top) {
    debug_print("create, beginning\n");
    rrb_t *rrb = malloc(sizeof *rrb);
    rrb->level = 1;
    rrb->ref = 1;
    rrb->elements = 0;
    rrb->full = false;
    rrb->meta = NULL;
    if (top == true) {
        make_nodes(rrb);
    } else {
        make_datas(rrb);
    }
    debug_print("create, end\n");
    return rrb;
}

/** Creates a RRB-Tree with datas. */
rrb_t *create_w_leafs() {
    debug_print("create_w_leafs\n");
    return create(false);
}

/** Creates a RRB-Tree with nodes. */
rrb_t *create_w_nodes() {
    debug_print("create_w_nodes\n");
    return create(true);
}

/** Checks if rrb contains data. */
bool contains_leafs(const rrb_t *rrb) {
    debug_print("contains_leafs\n");
    return rrb->leafs == true;
}

/** Checks if rrb contains nodes. */
bool contains_nodes(const rrb_t *rrb) {
    debug_print("contains_nodes\n");
    return rrb->leafs == false;
}

/** Creates an empty RRB-Vector. */
rrb_t *rrb_create() {
    debug_print("rrb_create\n");
    return create_w_leafs();
}

/** Frees a RRB-Tree. */
void free_rrb(rrb_t *rrb) {
    debug_print("free_rrb, beginning\n");
    if (rrb->meta != NULL) {
        free(rrb->meta);
    }
    if (contains_leafs(rrb)) {
        free(rrb->nodes.leaf);
    } else {
        free(rrb->nodes.child);
    }
    free(rrb);
    debug_print("free_rrb, end\n");
}

/** Increases references to the tree. */
rrb_t *inc_ref(rrb_t *rrb) {
    debug_print("inc_ref, beginning\n");
    rrb->ref += 1;
    debug_print("inc_ref, end\n");
    return rrb;
}

/** Decreases references to the tree. */
void dec_ref(rrb_t *rrb) {
    debug_print("dec_ref, beginning\n");
    rrb->ref -= 1;
    if (rrb->ref == 0) {
        if (contains_nodes(rrb)) {
            for (int i = 0; i < 32; i++) {
                if (rrb->nodes.child[i] != NULL) {
                    dec_ref(rrb->nodes.child[i]);
                }
            }
        }
        free_rrb(rrb);
    }
    debug_print("dec_ref, end\n");
}

/** Checks if a RRB-Tree is full. */
bool is_full(const rrb_t *rrb) {
    debug_print("is_full\n");
    return rrb->full == true;
}

/** Easily clone the informations. */
void clone_info(rrb_t *clone, const rrb_t *src) {
    debug_print("clone_info, beginning\n");
    clone->level = src->level;
    clone->elements = src->elements;
    clone->full = src->full;
    clone->leafs = src->leafs;
    debug_print("clone_info, end\n");
}

/** Easily clone the meta section. */
void clone_meta(rrb_t *clone, const rrb_t *src) {
    debug_print("clone_meta, beginning\n");
    if (src->meta != NULL) {
        clone->meta = malloc(sizeof *clone->meta * 32);
        for (int i = 0; i < 32; i++) {
            clone->meta[i] = src->meta[i];
        }
    } else {
        clone->meta = NULL;
    }
    debug_print("clone_meta, end\n");
}

/** Easily clone the nodes section. */
void clone_nodes(rrb_t *clone, const rrb_t *src) {
    debug_print("clone_nodes, beginning\n");
    if (contains_leafs(src)) {
        clone->nodes.leaf = malloc(sizeof *clone->nodes.leaf * 32);
        for (int i = 0; i < 32; i++) {
            clone->nodes.leaf[i] = src->nodes.leaf[i];
        }
    } else {
        clone->nodes.child = malloc(sizeof *clone->nodes.child * 32);
        for (int i = 0; i < 32; i++) {
            clone->nodes.child[i] = src->nodes.child[i];
            if (clone->nodes.child[i] != NULL) {
                inc_ref(clone->nodes.child[i]);
            }
        }
    }
    debug_print("clone_nodes, end\n");
}

/** Copies the node as is. */
rrb_t *copy_node(const rrb_t *src) {
    debug_print("copy_node, beginning\n");
    rrb_t *clone = malloc(sizeof *clone);
    clone->ref = 1;
    clone_info(clone, src);
    clone_meta(clone, src);
    clone_nodes(clone, src);
    debug_print("copy_node, end\n");
    return clone;
}

/** Inserts the data at the desired level. */
rrb_t *create_child(int level, imc_data_t *data) {
    debug_print("create_level, beginning\n");
    if (level == 1) {
        rrb_t *rrb = create_w_leafs();
        rrb->nodes.leaf[0] = data;
        rrb->elements = 1;
        rrb->level = level;
        debug_print("create_level, end\n");
        return rrb;
    } else {
        rrb_t *rrb = create_w_nodes();
        rrb->nodes.child[0] = create_child(level - 1, data);
        rrb->elements = 1;
        rrb->level = level;
        debug_print("create_level, end\n");
        return rrb;
    }
}

/** Adds a node, parent of the tree, and insert
* the data at the correct level. */
rrb_t *add_as_parent_to(rrb_t *child, imc_data_t *data) {
    debug_print("add_as_parent_to, beginning\n");
    rrb_t *parent = create_w_nodes();
    parent->level = child->level + 1;
    parent->nodes.child[0] = inc_ref(child);
    parent->nodes.child[1] = create_child(child->level, data);
    parent->elements = rrb_size(child) + 1;
    if (contains_nodes(child) && child->meta != NULL) {
        parent->meta = malloc(sizeof *parent->meta * 32);
        parent->meta[0] = rrb_size(child);
        parent->meta[1] = rrb_size(child) + 1;
    }
    debug_print("add_as_parent_to, end\n");
    return parent;
}

/** Calc the position according to the formula of research paper. */
int calc_position(int index, int level) {
    debug_print("calc_position\n");
    return (index >> (5 * (level - 1))) & 31;
}

/** Finds the correct place to insert the new data. */
int place_to_insert(const rrb_t *rrb, bool meta) {
    debug_print("place_to_insert, beginning\n");
    if (rrb->meta == NULL || meta == false) {
        return calc_position(rrb->elements, rrb->level);
    }

    for (int i = 0; i < 32; i++) {
        if (contains_nodes(rrb)) {
            if (rrb->nodes.child[i] == NULL || !is_full(rrb->nodes.child[i])) {
                debug_print("place_to_insert, leafs\n");
                return i;
            }
        } else {
            if (rrb->nodes.leaf[i] == NULL) {
                debug_print("place_to_insert, node\n");
                return i;
            }
        }
    }

    debug_print("place_to_insert, default case\n");
    return -1;
}

/** Adjusts the index in case of existing meta. */
int check_meta_index(const rrb_t *rrb, int* index) {
    debug_print("check_meta_index, beginning\n");
    debug_args("check_meta_index, index: %d\n", *index);
    for (int i = 0; i < 32; i++) {
        if (contains_nodes(rrb)) {
            if ((unsigned int) (*index) < rrb_size(rrb->nodes.child[i])) {
                return i;
            } else {
                *index -= rrb_size(rrb->nodes.child[i]);
            }
        } else {
            if (*index == 0) {
                return i;
            } else {
                *index -= 1;
            }
        }
    }
    return -1;
}

/** Gets the index needed to look at the right level. */
int place_to_look(const rrb_t *rrb, int* index, bool meta) {
    debug_print("place_to_look, beginning\n");
    if (rrb->meta != NULL || meta == true) {
        debug_print("place_to_look, check_meta_index\n");
        return check_meta_index(rrb, index);
    } else {
        debug_print("place_to_look, meta null\n");
        return calc_position(*index, rrb->level);
    }
}

/** Copies if the node exists, else creates it at the correct level. */
rrb_t *create_clone(const rrb_t *src, int level) {
    debug_print("create_clone, beginning\n");
    rrb_t *clone;
    if (src == NULL) {
        if (level == 1) {
            clone = create_w_leafs();
        } else {
            clone = create_w_nodes();
        }
        clone->level = level;
    } else {
        clone = copy_node(src);
    }
    debug_print("create_clone, end\n");
    return clone;
}

rrb_t *add_leaf(rrb_t *rrb, imc_data_t *data, int where);
rrb_t *add_node(rrb_t *rrb, imc_data_t *data, bool meta);

/** Adds a node to a non fully tree. */
rrb_t *add(rrb_t *clone, imc_data_t *data, bool meta) {
    debug_print("add, beginning\n");
    if (contains_leafs(clone)) {
        debug_print("add, leafs\n");
        return add_leaf(clone, data, place_to_insert(clone, meta));
    } else {
        debug_print("add, node\n");
        return add_node(clone, data, meta);
    }
}

/** Easily add a data to a tree leafs. */
rrb_t *add_leaf(rrb_t *rrb, imc_data_t *data, int where) {
    debug_print("add_leaf, beginnning\n");
    rrb->nodes.leaf[where] = data;
    if (where == 31) {
        rrb->full = true;
    }
    rrb->elements += 1;
    debug_print("add_leaf, end\n");
    return rrb;
}

/** Easily adds a data to a tree node. */
rrb_t *add_node(rrb_t *rrb, imc_data_t *data, bool meta) {
    debug_print("add_node, beginning\n");
    int where = place_to_insert(rrb, meta);
    rrb_t *clone = create_clone(rrb->nodes.child[where], rrb->level - 1);
    rrb_t *child = rrb->nodes.child[where];
    rrb->nodes.child[where] = add(clone, data, meta);
    if (child != NULL) {
        dec_ref(child);
    }
    if (where == 31 && is_full(rrb->nodes.child[where])) {
        rrb->full = true;
    }
    rrb->elements += 1;
    debug_print("add_node, end\n");
    return rrb;
}

/** Adds a data to the tree, and returns a new version of the tree. */
rrb_t *rrb_push(rrb_t *rrb, imc_data_t *data) {
    debug_print("rrb_push, beginning\n");
    if (is_full(rrb)) {
        debug_print("rrb_push, full\n");
        return add_as_parent_to(rrb, data);
    } else {
        debug_print("rrb_push, not full\n");
        rrb_t *clone = create_clone(rrb, rrb->level);
        if (rrb->meta == NULL) {
            return add(clone, data, false);
        } else {
            return add(clone, data, true);
        }
    }
}

/** Gets the size of rrb. */
size_t rrb_size(const rrb_t *rrb) {
    debug_print("rrb_size, beginning\n");
    if (rrb == NULL) {
        debug_print("rrb_size, null\n");
        return -1;
    } else {
        debug_print("rrb_size, not null\n");
        return rrb->elements;
    }
}

/** Looks for a data into the tree. */
imc_data_t *lookup(const rrb_t* rrb, int* index, bool meta) {
    debug_print("lookup, beginning\n");
    int position = place_to_look(rrb, index, meta);
    debug_args("lookup, position: %d\n", position);
    if (contains_nodes(rrb)) {
        debug_print("lookup, nodes\n");
        return lookup(rrb->nodes.child[position], index, meta);
    } else {
        debug_print("lookup, leafs\n");
        return rrb->nodes.leaf[position];
    }

}

/** Checks if the index is correct then looks for a data into the tree. */
imc_data_t *rrb_lookup(const rrb_t *rrb, int index) {
    debug_print("rrb_lookup, beginning\n");
    if (index >= rrb->elements) {
        debug_print("rrb_lookup, no index\n");
        return NULL;
    } else {
        debug_print("rrb_lookup, lookup\n");
        if (rrb->meta != NULL) {
            return lookup(rrb, &index, true);
        } else {
            return lookup(rrb, &index, false);
        }
    }
}

/** Unref rrb and free it automatically if needed. */
void rrb_unref(rrb_t *rrb) {
    debug_print("rrb_unref, beginning\n");
    dec_ref(rrb);
    debug_print("rrb_unref, end\n");
}

rrb_t *update_leaf(rrb_t *rrb, int  where, imc_data_t *data);
rrb_t *update_node(rrb_t *rrb, int *index, imc_data_t *data, bool meta);

/** Updates the tree by changing the data at index by data. */
rrb_t *update(const rrb_t *rrb, int *index, imc_data_t *data, bool meta) {
    debug_print("update, beginning\n");
    rrb_t *clone = create_clone(rrb, rrb->level);
    if (contains_leafs(clone)) {
        debug_print("update, leafs\n");
        return update_leaf(clone, place_to_look(clone, index, meta), data);
    } else {
        debug_print("update, node\n");
        return update_node(clone, index, data, meta);
    }
}

/** Easily updates a data in a tree leaf. */
rrb_t *update_leaf(rrb_t *rrb, int where, imc_data_t *data) {
    debug_print("update_leaf, beginnning\n");
    rrb->nodes.leaf[where] = data;
    if (where == 31) {
        rrb->full = true;
    }
    rrb->elements += 1;
    debug_print("add_leaf, end\n");
    return rrb;
}

/** Easily updates a data in a tree node. */
rrb_t *update_node(rrb_t *rrb, int *index, imc_data_t *data, bool meta) {
    debug_print("update_node, beginning\n");
    int where = place_to_look(rrb, index, meta);
    dec_ref(rrb->nodes.child[where]);
    rrb->nodes.child[where] = update(rrb->nodes.child[where], index, data, meta);
    if (where == 31 && is_full(rrb->nodes.child[where])) {
        rrb->full = true;
    }
    rrb->elements += 1;
    debug_print("update_node, end\n");
    return rrb;
}

/** Checks if index is inside the tree, and update the data at index by data. */
rrb_t *rrb_update(const rrb_t *rrb, int index, imc_data_t *data) {
    debug_print("rrb_update, beginning\n");
    if (index >= rrb->elements) {
        debug_print("rrb_lookup, no index\n");
        return NULL;
    } else {
        debug_print("rrb_update, update\n");
        if (rrb->meta != NULL) {
            return update(rrb, &index, data, true);
        } else {
            return update(rrb, &index, data, false);
        }
    }
}

rrb_t *pop_node(rrb_t *rrb, imc_data_t **data, int *index, bool meta);
rrb_t *pop_data(rrb_t *rrb, imc_data_t **data, int *index, bool meta);

/** Pop the last element for the tree, and put the data into data. */
rrb_t *pop(const rrb_t* rrb, imc_data_t **data, int *index, bool meta) {
    debug_print("pop, beginning\n");
    rrb_t *clone = create_clone(rrb, rrb->level);
    if (contains_nodes(rrb)) {
        return pop_node(clone, data, index, meta);
    } else {
        return pop_data(clone, data, index, meta);
    }
}

/** Convenient way to pop data from leafs. */
rrb_t *pop_data(rrb_t *rrb, imc_data_t **data, int *index, bool meta) {
    int position = place_to_look(rrb, index, meta);
    *data = rrb->nodes.leaf[position];
    rrb->nodes.leaf[position] = NULL;
    rrb->elements -= 1;
    if (rrb_size(rrb) == 0) {
        dec_ref(rrb);
        return NULL;
    } else {
        return rrb;
    }
}

/** Convenient way to pop data from nodes. */
rrb_t *pop_node(rrb_t *rrb, imc_data_t **data, int *index, bool meta) {
    int position = place_to_look(rrb, index, meta);
    rrb_t *child = pop(rrb->nodes.child[position], data, index, meta);
    dec_ref(rrb->nodes.child[position]);
    rrb->nodes.child[position] = child;
    return rrb;
}

/** Checks if the meta is NULL and pop the last data from the tree. */
rrb_t *rrb_pop(rrb_t *rrb, imc_data_t **data) {
    debug_print("rrb_pop, beginning\n");
    int index = rrb_size(rrb) - 1;
    // Check if there's at least an element in the tree.
    if (index == -1) {
        return NULL;
    }
    if (rrb->meta == NULL) {
        debug_print("rrb_pop, meta null\n");
        return pop(rrb, data, &index, false);
    } else {
        debug_print("rrb_pop, meta\n");
        return pop(rrb, data, &index, true);
    }
}
