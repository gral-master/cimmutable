/*******************************************************************************
  MIT License

  Copyright (c) 2016 gral-master

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*******************************************************************************/

/**
 * @file rrb_vect.h
 * @author Anastasios DOUMOULAKIS
 * @author Tanguy Retail
 * @brief C implementation of Immutable Relaxed Radix Balanced Vectors
 * @copyright MIT Licence
 *
 * This file defines the structures and functions needed to manipulate immutable
 * RRB vectors. The implementation is made from scratch but the strucute itself
 * comes from Nicolas Tucki's thesis.
 * @see https://github.com/nicolasstucki/scala-rrb-vector
 */

#ifndef _IMC_RRB_VECT_H
#define _IMC_RRB_VECT_H

#include "../../include/vector.h"
//-I -L à la compile pour faire #include <vector.h>
#include <stdio.h>

/**
 * @brief Size of the RRB arrays (should be a power of 2)
 */
#define ARRAY_SIZE 32
/**
 * @brief Maximum depth that the RRB can achieve
 */
#define MAX_DEPTH 7


/**
 * @brief The main structure representing an RRB node or leaf
 */
typedef struct imc_rrb {
    int floor; ///< Level of the node, 0 is a leaf 
    int refs; ///< Number of references pointing to this node
    int length; ///< Number of elements contained in the node
    int* meta; ///< Meta data for unbalanced trees
    union {
      struct imc_rrb** next; ///< Child nodes if the node is not a leaf
      imc_data_t** data; ///< Data stored if the node is a leaf
    } node; ///< Either the children or the data
} imc_rrb_t;


/*******************************/
/* Creation and initialization */
/*******************************/

/**
 * @brief Creates and allocates an empty RRB node.
 * @author Anastasios DOUMOULAKIS
 * @author Tanguy Retail
 * @return A new empty RRB node
 */
imc_rrb_t* imc_rrb_create();

/**
 * @brief Creates and allocates an empty RRB leaf.
 * @authors Anastasios DOUMOULAKIS
 * @authors Tanguy Retail
 * @return A new empty RRB leaf
 */
imc_rrb_t* imc_rrb_create_leaf();

/**
 * @brief Allocates an existing RRB with initial values.
 * @authors Anastasios DOUMOULAKIS
 * @authors Tanguy Retail
 * @param vec : The pointer to an existing RRB structure
 * @return The input RRB with its values initialized
 */
int imc_rrb_init(imc_rrb_t* vec);

/*****************/
/* rrb utilities */
/*****************/

/**
 * @brief Returns the number of elements in an RRB
 * @authors Anastasios Doumoulakis
 * @authors Tanguy Retail
 * @param vec : The pointer to the an existing RRB structure
 * @return The size of the RRB or 0 if vec is NULL
 */
int imc_rrb_size(imc_rrb_t* vec);


/**
 * @brief Returns whether an RRB is full or not
 * @authors Anastasios Doumoulakis
 * @authors Tanguy Retail
 * @param vec : The pointer to the an existing RRB structure 
 * @return 1 : The RRB is full 
 * @return 0 : The RRB is not full
 */
int imc_rrb_is_full(imc_rrb_t* vec);

/**
 * @brief Returns whether an RRB is balanced or not
 * @authors Anastasios Doumoulakis
 * @authors Tanguy Retail
 * @param vec : The pointer to the an existing RRB structure 
 * @return 1 : The RRB is balanced
 * @return 0 : The RRB is not balanced
 */
int imc_rrb_is_balanced(imc_rrb_t* vec);

/**
 * @brief Returns whether an RRB is a leaf or not
 * @authors Anastasios Doumoulakis
 * @authors Tanguy Retail
 * @param vec : The pointer to the an existing RRB structure 
 * @return 1 : The RRB is a leaf
 * @return 0 : The RRB is not a leaf
 */
int imc_rrb_is_leaf(imc_rrb_t* vec);


/**
 * @brief Return the last non-NULL child and set res_index if not null
 * @authors Anastasios Doumoulakis
 * @authors Tanguy Retail
 * @param vec : The pointer to the an existing RRB structure
 * @param res_index : Pointer to an int that will contain
 * the index of the last non-NULL child
 * @return The last non-NULL child or NULL if it is not null
 */
imc_rrb_t* imc_rrb_tail(imc_rrb_t* vec, int* res_index);


/**
 * @brief Return the first non-NULL child and set res_index if not null
 * @authors Anastasios Doumoulakis
 * @authors Tanguy Retail
 * @param vec : The pointer to the an existing RRB structure
 * @param res_index : Pointer to an int that will contain
 * the index of the first non-NULL child
 * @return The first non-NULL child or NULL if it is not null
 */
imc_rrb_t* imc_rrb_head(imc_rrb_t* vec, int* res_index);

/**
 * @brief Creates a new vector with the input as its first child 
 * @authors Anastasios Doumoulakis
 * @authors Tanguy Retail
 * @param vec : The pointer to the an existing RRB structure
 * @return A RRB with the input RRB as its first child
 */
imc_rrb_t* imc_rrb_new_root(imc_rrb_t* vec);

/**
 * @brief Increases the length of a rrb and updates the meta information    
 * @authors Anastasios Doumoulakis
 * @authors Tanguy Retail
 * @param vec : The pointer to the an existing RRB structure
 * @return void 
 */
void imc_rrb_increase_length(imc_rrb_t* vec, int index);

/**
 * @brief Decreases the length of a rrb and updates the meta information
 * @authors Anastasios Doumoulakis
 * @authors Tanguy Retail
 * @param vec : The pointer to the an existing RRB structure
 * @return void 
 */
void imc_rrb_decrease_length(imc_rrb_t* vec, int index);

/**
 * @brief Computes the subindex to follow to find the desired index 
 * @authors Anastasios Doumoulakis
 * @authors Tanguy Retail
 * @param vec : The pointer to the an existing RRB structure
 * @param index : The index of the element we wish to find
 * @return -1 : The index was not found in the RRB
 * @return int : The index of the sub tree where we will find index
 */
int imc_rrb_subindex(imc_rrb_t* vec, int index);

imc_data_t* imc_rrb_get_leaf_no_copy(imc_rrb_t* vec, int index);

int imc_rrb_fill_fifo(imc_rrb_t* rrb, imc_rrb_t** fifo,
                      int i_fifo, int ignore);

/**************/
/* Xtra tools */
/**************/

char* concatc(char* str, char c);

/*******************/
/* Copy operations */
/*******************/

imc_rrb_t* imc_rrb_copy_leaf(imc_rrb_t* vec);

imc_rrb_t* imc_rrb_copy(imc_rrb_t* vec);

/******************/
/* Push operation */
/******************/

imc_rrb_t* imc_rrb_push(imc_rrb_t* vec, imc_data_t* data);

imc_rrb_t* imc_rrb_push_full(imc_rrb_t* vec, imc_data_t* data);

imc_rrb_t* imc_rrb_push_not_full(imc_rrb_t* vec, imc_data_t* data);


/*****************/
/* Pop operation */
/*****************/

imc_rrb_t* imc_rrb_pop(imc_rrb_t* vec, imc_data_t** data);

/********************/
/* Update operation */
/********************/

imc_rrb_t* imc_rrb_update(imc_rrb_t* vec, int index, imc_data_t* data);

/********************/
/* Lookup operation */
/********************/

imc_data_t* imc_rrb_lookup(imc_rrb_t* vec, int index);

/*******************/
/* Split operation */
/*******************/

int imc_rrb_split(imc_rrb_t* vec_in, int index, imc_rrb_t** vec_out1,
                  imc_rrb_t** vec_out2);

void imc_rrb_build_left(imc_rrb_t* vec_in, imc_rrb_t* left,
                        int index);

void imc_rrb_build_right(imc_rrb_t* vec_in, imc_rrb_t* right,
                         int index);

/*******************/
/* Merge operation */
/*******************/

imc_rrb_t* imc_rrb_merge(imc_rrb_t* vec_front, imc_rrb_t* vec_tail);

imc_rrb_t* compress(imc_rrb_t* left, imc_rrb_t* mid, imc_rrb_t* right, 
                    int ignore1, int ignore2, int last_step);

int elem_on_floor(imc_rrb_t* rrb);

imc_rrb_t* imc_rrb_merge_nodes(imc_rrb_t* left, imc_rrb_t* middle,
                               imc_rrb_t* right, int ignore1, 
                               int ignore2, int last_step);

imc_rrb_t* imc_rrb_merge_leaves(imc_rrb_t* vec1, imc_rrb_t* vec2);

/*******************/
/* Unref operation */
/*******************/

int imc_rrb_unref(imc_rrb_t* vec);

/********************/
/* Print operations */
/********************/

void imc_rrb_dump(imc_rrb_t* vec);

void imc_rrb_emit(imc_rrb_t* vec, const char* path, 
                  char* (*print)(imc_data_t*));

void emit_node(imc_rrb_t* vec, char* from, char* prefix, FILE* fp,
               char* (*print)(imc_data_t*), int refs);
#endif
