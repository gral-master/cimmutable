#include "rrb_vector.h"

#define DEBUG 0

void make_children(rrb_vector_t *rrb) {
    debug_print("make_children, beginning\n");
    rrb->leafs = false;
    rrb->children.arr = calloc(32, sizeof *rrb->children.arr);
    debug_print("make_children, end\n");
}

void make_datas(rrb_vector_t *rrb) {
    debug_print("make_datas, beginning\n");
    rrb->leafs = true;
    rrb->children.data = calloc(32, sizeof *rrb->children.data);
    debug_print("make_datas, end\n");
}

/** Creates an empty RRB-Tree. Top is used in order to determine if it contains
  * leafs or RRB-Nodes. */
rrb_vector_t *create(bool top) {
    debug_print("create, beginning\n");
    rrb_vector_t *rrb = malloc(sizeof *rrb);
    rrb->level = 1;
    rrb->ref = 1;
    rrb->elements = 0;
    rrb->full = false;
    rrb->meta = NULL;
    if (top == true) {
        make_children(rrb);
    } else {
        make_datas(rrb);
    }
    debug_print("create, end\n");
    return rrb;
}

/** Creates an empty RRB-Vector. */
rrb_vector_t *rrb_create() {
    debug_print("rrb_create\n");
    return create(false);
}

void free_rrb(rrb_vector_t *rrb) {
    debug_print("free_rrb, beginning\n");
    if (rrb->meta != NULL) {
        free(rrb->meta);
    }
    if (rrb->leafs) {
        free(rrb->children.data);
    } else {
        free(rrb->children.arr);
    }
    free(rrb);
    debug_print("free_rrb, end\n");
}

/** Increases references to the tree. */
void inc_ref(rrb_vector_t *rrb) {
    debug_print("inc_ref, beginning\n");
    rrb->ref += 1;
    debug_print("inc_ref, end\n");
}

/** Decreases references to the tree. */
void dec_ref(rrb_vector_t *rrb) {
    debug_print("dec_ref, beginning\n");
    rrb->ref -= 1;
    if (rrb->ref == 0) {
        if (rrb->leafs == false) {
            for (int i = 0; i < 32; i++) {
                if (rrb->children.arr[i] != NULL) {
                    dec_ref(rrb->children.arr[i]);
                }
            }
        }
        free_rrb(rrb);
    }
    debug_print("dec_ref, end\n");
}

bool is_full(const rrb_vector_t *rrb) {
    debug_print("is_full\n");
    return rrb->full == true;
}

/** Finds the max level of the tree. */
int find_max_level(const rrb_vector_t *rrb) {
    debug_print("find_max_level, beginning\n");
    if (rrb->leafs == true) {
        debug_print("find_max_level, leaf\n");
        return rrb->level;
    } else {
        debug_print("find_max_level, else\n");
        return find_max_level(rrb->children.arr[0]);
    }
}

/** Easily clone the informations. */
void clone_info(rrb_vector_t *clone, const rrb_vector_t *src) {
    debug_print("clone_info, beginning\n");
    clone->level = src->level;
    clone->elements = src->elements;
    clone->full = src->full;
    clone->leafs = src->leafs;
    debug_print("clone_info, end\n");
}

/** Easily clone the meta section. */
void clone_meta(rrb_vector_t *clone, const rrb_vector_t *src) {
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

/** Easily clone the children section. */
void clone_children(rrb_vector_t *clone, const rrb_vector_t *src) {
    debug_print("clone_children, beginning\n");
    if (src->leafs == true) {
        clone->children.data = malloc(sizeof *clone->children.data * 32);
        for (int i = 0; i < 32; i++) {
            clone->children.data[i] = src->children.data[i];
        }
    } else {
        clone->children.arr = malloc(sizeof *clone->children.arr * 32);
        for (int i = 0; i < 32; i++) {
            clone->children.arr[i] = src->children.arr[i];
            if (clone->children.arr[i] != NULL) {
                inc_ref(clone->children.arr[i]);
            }
        }
    }
    debug_print("clone_children, end\n");
}

/** Copies the node as is. */
rrb_vector_t *copy_node(const rrb_vector_t *src) {
    debug_print("copy_node, beginning\n");
    rrb_vector_t *clone = malloc(sizeof *clone);
    clone->ref = 1;
    clone_info(clone, src);
    clone_meta(clone, src);
    clone_children(clone, src);
    debug_print("copy_node, end\n");
    return clone;
}

/** Copies the tree and increases every level. */
rrb_vector_t *copy_and_increase(const rrb_vector_t *src) {
    debug_print("copy_and_increase, beginning\n");
    rrb_vector_t *clone = copy_node(src);
    clone->level += 1;
    if (clone->leafs == false) {
        for (int i = 0; i < 32; i++) {
            dec_ref(clone->children.arr[i]);
            clone->children.arr[i] = copy_and_increase(clone->children.arr[i]);
        }
    }
    debug_print("copy_and_increase, end\n");
    return clone;
}

/** Creates the parents of a leafs node. */
rrb_vector_t *create_parents(int level, rrb_vector_t *child) {
    debug_print("create_parents, beginning\n");
    if (level == 1) {
        debug_print("create_parents, child\n");
        return child;
    } else {
        rrb_vector_t *rrb = create(true);
        rrb->children.arr[0] = child;
        rrb->elements = 1;
        rrb->level = level;
        debug_print("create_parents, parents\n");
        return create_parents(level - 1, rrb);
    }
}

/** Inserts the data at the desired level. */
rrb_vector_t *create_level(int level, imc_data_t *data) {
    debug_print("create_level, beginning\n");
    rrb_vector_t *rrb = create(false);
    rrb->children.data[0] = data;
    rrb->elements = 1;
    rrb->level = level;
    debug_print("create_level, end\n");
    return create_parents(level - 1, rrb);
}

/** Adds a node, parent of the tree, and insert
  * the data at the correct level. */
rrb_vector_t *add_as_parent_to(const rrb_vector_t *child, imc_data_t *data) {
    debug_print("add_as_parent_to, beginning\n");
    rrb_vector_t *parent = create(true);
    parent->children.arr[0] = copy_and_increase(child);
    parent->children.arr[1] = create_level(find_max_level(parent), data);
    parent->elements = parent->children.arr[0]->elements + 1;
    debug_print("add_as_parent_to, end\n");
    return parent;
}

rrb_vector_t *add_leaf(rrb_vector_t *rrb, imc_data_t *data, int where);
rrb_vector_t *add_node(rrb_vector_t *rrb, imc_data_t *data, int where, int level, int root);

int calc_position(int index, int level) {
    debug_print("calc_position\n");
    return index >> ((5 * level) & 31);
}

/** Finds the correct place to insert the new data. */
int place_to_insert(const rrb_vector_t *rrb) {
    debug_print("place_to_insert, beginning\n");
    if (rrb->meta == NULL) {
        debug_print("place_to_insert, meta null\n");
        return rrb->elements / pow(32, find_max_level(rrb) - rrb->level);
    } else {
        for (int i = 0; i < 32; i++) {
            if (rrb->leafs == true) {
                if (!is_full(rrb->children.arr[i])) {
                    debug_print("place_to_insert, leafs\n");
                    return i;
                }
            } else {
                if (rrb->children.data[i] == NULL) {
                    debug_print("place_to_insert, node\n");
                    return i;
                }
            }
        }
    }
}

/** Copies if the node exists, else creates it at the correct level. */
rrb_vector_t *clone_or_create(const rrb_vector_t *src, int level, int root) {
    debug_print("clone_or_create, beginning\n");
    rrb_vector_t *clone;
    if (src == NULL) {
        if (level == root) {
            clone = create(false);
        } else {
            clone = create(true);
        }
        clone->level = level;
    } else {
        clone = copy_node(src);
    }
    debug_print("clone_or_create, end\n");
    return clone;
}

/** Adds a node to a non fully tree. */
rrb_vector_t *add(const rrb_vector_t *src, imc_data_t *data, int level, int root) {
    debug_print("add, beginning\n");
    rrb_vector_t *clone = clone_or_create(src, level, root);
    if (clone->leafs == true) {
        debug_print("add, leafs\n");
        return add_leaf(clone, data, place_to_insert(clone));
    } else {
        debug_print("add, node\n");
        return add_node(clone, data, place_to_insert(clone), level, root);
    }
}

/** Easily add a data to a tree leafs. */
rrb_vector_t *add_leaf(rrb_vector_t *rrb, imc_data_t *data, int where) {
    debug_print("add_leaf, beginnning\n");
    rrb->children.data[where] = data;
    if (where == 31) {
        rrb->full = true;
    }
    rrb->elements += 1;
    debug_print("add_leaf, end\n");
    return rrb;
}

/** Easily adds a data to a tree node. */
rrb_vector_t *add_node(rrb_vector_t *rrb, imc_data_t *data, int where, int level, int root) {
    debug_print("add_node, beginning\n");
    if (rrb->children.arr[where] != NULL) {
        dec_ref(rrb->children.arr[where]);
    }
    rrb->children.arr[where] = add(rrb->children.arr[where], data, level + 1, root);
    if (where == 31) {
        rrb->full = true;
    }
    rrb->elements += 1;
    debug_print("add_node, end\n");
    return rrb;
}

/** Adds a data to the tree, and returns a new version of the tree. */
rrb_vector_t *rrb_add(const rrb_vector_t *rrb, imc_data_t *data) {
    debug_print("rrb_add, beginning\n");
    if (is_full(rrb)) {
        debug_print("rrb_add, full\n");
        return add_as_parent_to(rrb, data);
    } else {
        debug_print("rrb_add, not full\n");
        return add(rrb, data, 1, find_max_level(rrb));
    }
}

size_t rrb_size(const rrb_vector_t *rrb) {
    debug_print("rrb_size\n");
    return rrb->elements;
}

imc_data_t *rrb_lookup(rrb_vector_t *rrb, int index) {
    return NULL;
}
