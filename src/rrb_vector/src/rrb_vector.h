#pragma once

// #include <vector.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef int imc_data_t;

typedef struct _rrb {
  int level;    // Depth of Node.
  int ref;      // Number of elements pointing to it.
  int elements; // Number of elements contained.
  int *meta;    // Indicates if relaxed tree.
  bool full;    // Indicates if the node is full.
  bool leafs;   // Indicates if terminal node.
  union {
    struct _rrb **arr;
    imc_data_t **data;
  } children;   // Contains the lefs or the nodes.
} rrb_vector_t;

/**
 * Prints the string provided if debug mode enabled.
 * @param  fmt The string which must be printed.
 * @return     None.
 */
#define debug_print(fmt)                       \
    do {                                       \
        if (DEBUG) {                           \
            fprintf(stderr, fmt);              \
        }                                      \
    } while (0)

/**
 * Prints the string provided if debug mode enabled. Handle multiple args.
 * @param  fmt     The string which must be printed.
 * @param  VARARGS The other arguments.
 * @return         None.
 */
#define debug_args(fmt, ...)                   \
    do {                                       \
        if (DEBUG) {                           \
            fprintf(stderr, fmt, __VA_ARGS__); \
        }                                      \
    } while (0)

/**
 * Creates an RRB-Tree.
 * @return A newly created RRB-Tree.
 */
rrb_vector_t *rrb_create();

/**
 * Add an element to an RRB-Tree. As RRBs are immutable, a new version
 * is created and returned by the function.
 * @param  rrb  The RRB-Tree.
 * @param  data The data to insert into the RRB.
 * @return      A new RRB-Tree containing the data.
 */
rrb_vector_t *rrb_add(rrb_vector_t *rrb, imc_data_t *data);

/**
 * Takes an RRB-Tree, updates the data contained at the corresponding index,
 * and returns the corresponding new RRB.
 * @param  rrb   The RRB-Tree to update.
 * @param  index The index of the element to change.
 * @param  data  The new data which have to be put at index.
 * @return       The new corresponding RRB-Tree.
 */
rrb_vector_t *rrb_update(const rrb_vector_t *rrb, int index, imc_data_t *data);

/**
 * Looks for an element at the corresponding index into an RRB-Tree.
 * @param  rrb   The RRB-Tree to look in.
 * @param  index The index of the element to look.
 * @return       The element if any, else NULL.
 */
imc_data_t *rrb_lookup(const rrb_vector_t *rrb, int index);

/**
 * Returns the size of an RRB-Tree.
 * @param  rrb The RRB-Tree to know the size.
 * @return     The size of the RRB-Tree if any, else -1.
 */
size_t rrb_size(const rrb_vector_t *rrb);

/**
 * Decreases the references to an RRB-Tree.
 * If an RRB-Tree has no more references, frees it. Accessing to an element after unref could not warranty what happened: probably a segmentation fault.
 * @param rrb The RRB-Tree to unref.
 */
void rrb_unref(rrb_vector_t *rrb);
