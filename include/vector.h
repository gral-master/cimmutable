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


#ifndef _VECTOR_H
#define _VECTOR_H

/*#define _STRUCT_HEADER()                                              \
    int (*size)(imc_vector_t*);                                         \
    imc_vector_t* (*update)(imc_vector_t*, int, imc_data_t*);           \
    imc_data_t* (*lookup)(imc_vector_t*, int);                          \
    imc_vector_t* (*push)(imc_vector_t*, imc_data_t*);                  \
    imc_vector_t* (*pop)(imc_vector_t*, imc_data_t**);                  \
    int (*imc_vector_split)(imc_vector_t*, int,                         \
                            imc_vector_t**, imc_vector_t**);            \
    imc_vector_t* (*imc_vector_merge)(imc_vector_t*, imc_vector_t*);    \
    int (*imc_vector_unref)(imc_vector_t*);                             \
    void (*imc_vector_dump)(imc_vector_t*);
*/

typedef struct imc_vector{} imc_vector_t;

typedef imc_data_t;

imc_vector_t* imc_vector_create();

int imc_vector_size(imc_vector_t* vec);

imc_vector_t* imc_vector_update(imc_vector_t* vec, int index, imc_data_t* data);

imc_data_t* imc_vector_lookup(imc_vector_t* vec, int index);

/* stack operations */

// add at the end <--- DOXYGENIZE PLEASE!
imc_vector_t* imc_vector_push(imc_vector_t* vec, imc_data_t* data);

imc_vector_t* imc_vector_pop(imc_vector_t* vec, imc_data_t** data);

int imc_vector_split(imc_vector_t* vec_in, int index,
                     imc_vector_t** vec_out1,
                     imc_vector_t** vec_out2);

imc_vector_t* imc_vector_merge(imc_vector_t* vec_front, imc_vector_t* vec_tail);

/* user-side memory management */

int imc_vector_unref(imc_vector_t* vec);

void imc_vector_dump(imc_vector_t* vec);

#endif
