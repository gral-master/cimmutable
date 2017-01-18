#pragma once

#include "rrb_vector.h"
#include <stdio.h>

/**
 * Dumps an RRB-Tree to a dotfile. Not working.
 * @param rrb  The RRB-Tree to dump.
 * @param path The path to write in.
 */
void rrb_to_dotfile(rrb_vector_t *rrb, char *path);

/**
 * Pretty print an RRB-Tree into the stdout.
 * @param rrb The RRB-Tree to pretty print.
 */
void rrb_pp(const rrb_vector_t *rrb);

/**
 * Pretty print an RRB-Tree into the file indicated by path.
 * @param rrb  The RRB-Tree to pretty print.
 * @param path The path to write in.
 */
void rrb_pp_file(const rrb_vector_t *rrb, char *path);

/**
 * Pretty print and RRB-Tree with pointers to the console.
 * @param rrb The RRB-Tree to pretty print.
 */
void rrb_ppp(const rrb_vector_t *rrb);

/**
 * Pretty print and RRB-Tree with pointers and leafs to the console.
 * @param rrb The RRB-Tree to pretty print.
 */
void rrb_ppp_leafs(const rrb_vector_t *rrb);

/**
 * Pretty print an RRB-Tree with pointers to the file indicated by path.
 * @param rrb  The RRB-Tree to pretty print.
 * @param path The path to write in.
 */
void rrb_ppp_file(const rrb_vector_t *rrb, char *path);

/**
 * Pretty print an RRB-Tree with pointers and leafs to the file indicated by path.
 * @param rrb  The RRB-Tree to pretty print.
 * @param path The path to write in.
 */
void rrb_ppp_leafs_file(const rrb_vector_t *rrb, char *path);
