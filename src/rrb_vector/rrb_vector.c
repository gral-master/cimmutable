#include "rrb_vector.h"

void make_children(rrb_vector_t *rrb) {
    rrb->leafs = false;
    rrb->children.arr = calloc(32, sizeof *rrb->children.arr);
}

void make_datas(rrb_vector_t *rrb) {
    rrb->leafs = true;
    rrb->children.data = calloc(32, sizeof *rrb->children.data);
}

/** Creates an empty RRB-Tree. Top is used in order to determine if it contains
  * leafs or RRB-Nodes. */
rrb_vector_t *create(bool top) {
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
    return rrb;
}

/** Creates an empty RRB-Vector. */
rrb_vector_t *rrb_create() {
    return create(false);
}

/** Increases references to the tree. */
void inc_ref(rrb_vector_t *rrb) {
    rrb->ref += 1;
}

/** Decreases references to the tree. */
void dec_ref(rrb_vector_t *rrb) {
    rrb->ref -= 1;
}

/** Finds the max level of the tree. */
int find_max_level(const rrb_vector_t *rrb) {
    if (rrb->leafs == true) {
        return rrb->level;
    } else {
        return find_max_level(rrb->children.arr[0]);
    }
}

/** Easily clone the informations. */
void clone_info(rrb_vector_t *clone, const rrb_vector_t *src) {
    clone->level = src->level;
    clone->elements = src->elements;
    clone->full = src->full;
    clone->leafs = src->leafs;
}

/** Easily clone the meta section. */
void clone_meta(rrb_vector_t *clone, const rrb_vector_t *src) {
    if (src->meta != NULL) {
        clone->meta = malloc(sizeof *clone->meta * 32);
        for (int i = 0; i < 32; i++) {
            clone->meta[i] = src->meta[i];
        }
    }
}

/** Easily clone the children section. */
void clone_children(rrb_vector_t *clone, const rrb_vector_t *src) {
    if (src->leafs == true) {
        clone->children.data = malloc(sizeof *clone->children.data * 32);
        for (int i = 0; i < 32; i++) {
            clone->children.data[i] = src->children.data[i];
        }
    } else {
        clone->children.arr = malloc(sizeof *clone->children.arr * 32);
        for (int i = 0; i < 32; i++) {
            clone->children.arr[i] = src->children.arr[i];
            inc_ref(clone->children.arr[i]);
        }
    }
}

/** Copies the node as is. */
rrb_vector_t *copy_node(const rrb_vector_t *src) {
    rrb_vector_t *clone = malloc(sizeof *clone);
    clone->ref = 1;
    clone_info(clone, src);
    clone_meta(clone, src);
    clone_children(clone, src);
    return clone;
}

/** Copies the tree and increases every level. */
rrb_vector_t *copy_and_increase(const rrb_vector_t *src) {
    rrb_vector_t *clone = copy_node(src);
    clone->level += 1;
    if (clone->leafs == false) {
        for (int i = 0; i < 32; i++) {
            dec_ref(clone->children.arr[i]);
            clone->children.arr[i] = copy_and_increase(clone->children.arr[i]);
        }
    }
    return clone;
}

/** Creates the parents of a leafs node. */
rrb_vector_t *create_parents(int level, rrb_vector_t *child) {
    if (level == 1) {
        return child;
    } else {
        rrb_vector_t *rrb = create(true);
        rrb->children.arr[0] = child;
        rrb->elements = 1;
        rrb->level = level;
        return create_parents(level - 1, rrb);
    }
}

/** Inserts the data at the desired level. */
rrb_vector_t *create_level(int level, imc_data_t *data) {
    rrb_vector_t *rrb = create(false);
    rrb->children.data[0] = data;
    rrb->elements = 1;
    rrb->level = level;
    return create_parents(level - 1, rrb);
}

/** Adds a node, parent of the tree, and insert the data at the correct level. */
rrb_vector_t *add_as_parent_to(const rrb_vector_t *child, imc_data_t *data) {
    rrb_vector_t *parent = create(true);
    parent->children.arr[0] = copy_and_increase(child);
    parent->children.arr[1] = create_level(find_max_level(parent), data);
    parent->elements = parent->children.arr[0]->elements + 1;
    return parent;
}

rrb_vector_t *add_leaf(rrb_vector_t *rrb, imc_data_t *data, int where);
rrb_vector_t *add_node(rrb_vector_t *rrb, imc_data_t *data, int where, int level, int root);

/** Finds the correct place to insert the new data. */
int place_to_insert(const rrb_vector_t *rrb) {
    return rrb->elements / pow(32, find_max_level(rrb) - rrb->level);
}

/** Copies if the node exists, else creates it at the correct level. */
rrb_vector_t *clone_and_null(const rrb_vector_t *src, int level, int root) {
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
        clone->elements += 1;
    }
    return clone;
}

/** Adds a node to a non fully tree. */
rrb_vector_t *add(const rrb_vector_t *src, imc_data_t *data, int level, int root) {
    rrb_vector_t *clone = clone_and_null(src, level, root);

    if (clone->leafs == true) {
        return add_leaf(clone, data, place_to_insert(clone));
    } else {
        return add_node(clone, data, place_to_insert(clone), level, root);
    }
}

/** Easily add a data to a tree leafs. */
rrb_vector_t *add_leaf(rrb_vector_t *rrb, imc_data_t *data, int where) {
    rrb->children.data[where] = data;
    if (where == 31) {
        rrb->full = true;
    }
    return rrb;
}

/** Easily adds a data to a tree node. */
rrb_vector_t *add_node(rrb_vector_t *rrb, imc_data_t *data, int where, int level, int root) {
    if (rrb->children.arr[where] != NULL) {
        dec_ref(rrb->children.arr[where]);
    }
    rrb->children.arr[where] = add(rrb->children.arr[where], data, level + 1, root);
    if (where == 31) {
        rrb->full = true;
    }
    return rrb;
}

/** Adds a data to the tree, and returns a new version of the tree. */
rrb_vector_t *rrb_add(const rrb_vector_t *rrb, imc_data_t *data) {
    if (rrb->full) {
        return add_as_parent_to(rrb, data);
    } else {
        return add(rrb, data, 1, find_max_level(rrb));
    }
}
