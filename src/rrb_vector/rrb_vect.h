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
 * @version 0.1
 * @author Anastasios DOUMOULAKIS
 * @author Tanguy RETAIL
 * @date 9 december 2016
 * @brief Immutable Relaxed Radix Balanced Vectors C implementation 
 * @copyright MIT Licence
 * 
 * This file defines the structures and functions needed to manipulate immutable 
 * RRB vectors. The implementation is made from scratch but the strucutre itself
 * comes from Nicolas Tucki's thesis.
 * @see https://github.com/nicolasstucki/scala-rrb-vector
 */

#ifndef _RRB_VECT_H
#define _RRB_VECT_H


/* ARRAY_SIZE should be a power of 2 */
#define ARRAY_SIZE 32
#define MAX_DEPTH 7

typedef imc_rrb_node_t;
//TODO define imc_data_t in a data.h file
typedef imc_data_t;

typedef struct imc_rrb {
    int max_depth;
    int m;
    imc_intern_t* root;
} imc_rrb_t;

typedef struct imc_rrb_intern {
    int level;
    int refs;
    int is_unbalanced;
    int* meta;
    imc_rrb_node_t* childs;
} imc_rrb_intern_t;

typedef struct imc_rrb_leaf {
    imc_data_t* data;
} imc_rrb_leaf_t;

typedef union imc_rrb_node {
    imc_intern_t intern;
    imc_leaf_t leaf; 
} imc_rrb_note_t;

imc_rrb_t* imc_vector_concrete_create();

imc_rrb_t* imc_vector_concrete_lookup(imc_rrb_t* vec, int index);

imc_rrb_t* imc_vector_push(imc_rrb_t* vec, imc_data_t* data);

imc_rrb_t* imc_vector_emmit(imc_rrb_t* vec);

#endif
