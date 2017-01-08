#include "rrb_vector.h"

#define DEBUG 1

/** Creates an empty array of nodes into rrb. */
void make_children(rrb_vector_t *rrb) {
    debug_print("make_children, beginning\n");
    rrb->leafs = false;
    rrb->children.arr = calloc(32, sizeof *rrb->children.arr);
    debug_print("make_children, end\n");
}

/** Creates an empty array of leafs into rrb. */
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

/** Creates a RRB-Tree with datas. */
rrb_vector_t *create_w_leafs() {
    debug_print("create_w_leafs\n");
    return create(false);
}

/** Creates a RRB-Tree with children. */
rrb_vector_t *create_w_children() {
    debug_print("create_w_children\n");
    return create(true);
}

/** Checks if rrb contains data. */
bool contains_leafs(const rrb_vector_t *rrb) {
    debug_print("contains_leafs\n");
    return rrb->leafs == true;
}

/** Checks if rrb contains nodes. */
bool contains_nodes(const rrb_vector_t *rrb) {
    debug_print("contains_nodes\n");
    return rrb->leafs == false;
}

/** Creates an empty RRB-Vector. */
rrb_vector_t *rrb_create() {
    debug_print("rrb_create\n");
    return create_w_leafs();
}

/** Frees a RRB-Tree. */
void free_rrb(rrb_vector_t *rrb) {
    debug_print("free_rrb, beginning\n");
    if (rrb->meta != NULL) {
        free(rrb->meta);
    }
    if (contains_leafs(rrb)) {
        free(rrb->children.data);
    } else {
        free(rrb->children.arr);
    }
    free(rrb);
    debug_print("free_rrb, end\n");
}

/** Increases references to the tree. */
rrb_vector_t *inc_ref(rrb_vector_t *rrb) {
    debug_print("inc_ref, beginning\n");
    rrb->ref += 1;
    debug_print("inc_ref, end\n");
    return rrb;
}

/** Decreases references to the tree. */
void dec_ref(rrb_vector_t *rrb) {
    debug_print("dec_ref, beginning\n");
    rrb->ref -= 1;
    if (rrb->ref == 0) {
        if (contains_nodes(rrb)) {
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

/** Checks if a RRB-Tree is full. */
bool is_full(const rrb_vector_t *rrb) {
    debug_print("is_full\n");
    return rrb->full == true;
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
    if (contains_leafs(src)) {
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

/** Inserts the data at the desired level. */
rrb_vector_t *create_child(int level, imc_data_t *data) {
    debug_print("create_level, beginning\n");
    if (level == 1) {
        rrb_vector_t *rrb = create_w_leafs();
        rrb->children.data[0] = data;
        rrb->elements = 1;
        rrb->level = level;
        debug_print("create_level, end\n");
        return rrb;
    } else {
        rrb_vector_t *rrb = create_w_children();
        rrb->children.arr[0] = create_child(level - 1, data);
        rrb->elements = 1;
        rrb->level = level;
        debug_print("create_level, end\n");
        return rrb;
    }
}

/** Adds a node, parent of the tree, and insert
  * the data at the correct level. */
rrb_vector_t *add_as_parent_to(rrb_vector_t *child, imc_data_t *data) {
    debug_print("add_as_parent_to, beginning\n");
    rrb_vector_t *parent = create_w_children();
    parent->level = child->level + 1;
    parent->children.arr[0] = inc_ref(child);
    parent->children.arr[1] = create_child(child->level, data);
    parent->elements = parent->children.arr[0]->elements + 1;
    debug_print("add_as_parent_to, end\n");
    return parent;
}

rrb_vector_t *add_leaf(rrb_vector_t *rrb, imc_data_t *data, int where);
rrb_vector_t *add_node(rrb_vector_t *rrb, imc_data_t *data, int where);

/** Calc the position according to the formula of research paper. */
int calc_position(int index, int level) {
    debug_print("calc_position\n");
    return (index >> (5 * (level - 1))) & 31;
}

/** Finds the correct place to insert the new data. */
int place_to_insert(const rrb_vector_t *rrb) {
    debug_print("place_to_insert, beginning\n");
    if (rrb->meta == NULL) {
        debug_print("place_to_insert, meta null\n");
        return calc_position(rrb->elements, rrb->level);
    } else {
        for (int i = 0; i < 32; i++) {
            if (contains_nodes(rrb)) {
                if (rrb->children.arr[i] == NULL || !is_full(rrb->children.arr[i])) {
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
    debug_print("place_to_insert, default case\n");
    return -1;
}

int check_meta_index(const rrb_vector_t *rrb, int index, int position) {
    debug_print("check_meta_index, beginning\n");
    if (position > 0) {
        while(rrb->meta[position - 1] > index) {
            position -= 1;
        }
    }
    if (position < 32) {
        while (rrb->meta[position] < index) {
            position += 1;
        }
    }
    debug_args("check_meta_index, position: %d\n", position);
    return position;
}

int place_to_look(const rrb_vector_t *rrb, int index) {
    debug_print("place_to_look, beginning\n");
    if (rrb->meta == NULL) {
        debug_print("place_to_look, meta null\n");
        return calc_position(index, rrb->level);
    } else {
        debug_print("place_to_look, check_meta_index\n");
        return check_meta_index(rrb, index, calc_position(index, rrb->level));
    }
}

/** Copies if the node exists, else creates it at the correct level. */
rrb_vector_t *clone_or_create(const rrb_vector_t *src, int level) {
    debug_print("clone_or_create, beginning\n");
    rrb_vector_t *clone;
    if (src == NULL) {
        if (level == 1) {
            clone = create_w_leafs();
        } else {
            clone = create_w_children();
        }
        clone->level = level;
    } else {
        clone = copy_node(src);
    }
    debug_print("clone_or_create, end\n");
    return clone;
}

/** Adds a node to a non fully tree. */
rrb_vector_t *add(const rrb_vector_t *src, imc_data_t *data, int level) {
    debug_print("add, beginning\n");
    rrb_vector_t *clone = clone_or_create(src, level);
    if (contains_leafs(clone)) {
        debug_print("add, leafs\n");
        return add_leaf(clone, data, place_to_insert(clone));
    } else {
        debug_print("add, node\n");
        return add_node(clone, data, place_to_insert(clone));
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
rrb_vector_t *add_node(rrb_vector_t *rrb, imc_data_t *data, int where) {
    debug_print("add_node, beginning\n");
    if (rrb->children.arr[where] != NULL) {
        dec_ref(rrb->children.arr[where]);
    }
    rrb->children.arr[where] = add(rrb->children.arr[where], data, rrb->level - 1);
    if (where == 31 && is_full(rrb->children.arr[where])) {
        rrb->full = true;
    }
    rrb->elements += 1;
    debug_print("add_node, end\n");
    return rrb;
}

/** Adds a data to the tree, and returns a new version of the tree. */
rrb_vector_t *rrb_add(rrb_vector_t *rrb, imc_data_t *data) {
    debug_print("rrb_add, beginning\n");
    if (is_full(rrb)) {
        debug_print("rrb_add, full\n");
        return add_as_parent_to(rrb, data);
    } else {
        debug_print("rrb_add, not full\n");
        return add(rrb, data, rrb->level);
    }
}

/** Gets the size of rrb. */
size_t rrb_size(const rrb_vector_t *rrb) {
    debug_print("rrb_size\n");
    return rrb->elements;
}

/** Loosks for a data into the tree. */
imc_data_t *lookup(const rrb_vector_t* rrb, int index) {
    debug_print("lookup, beginning\n");
    int position = place_to_look(rrb, index);
    debug_args("lookup, position: %d\n", position);
    if (contains_nodes(rrb)) {
        debug_print("lookup, nodes\n");
        return lookup(rrb->children.arr[position], index);
    } else {
        debug_print("lookup, leafs\n");
        return rrb->children.data[position];
    }

}

/** Checks if the index is correct then looks for a data into the tree. */
imc_data_t *rrb_lookup(const rrb_vector_t *rrb, int index) {
    debug_print("rrb_lookup, beginning\n");
    if (index >= rrb->elements) {
        debug_print("rrb_lookup, no index\n");
        return NULL;
    } else {
        debug_print("rrb_lookup, lookup\n");
        return lookup(rrb, index);
    }
}
