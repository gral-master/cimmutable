#ifndef _AVL_VECTOR
#define _AVL_VECTOR


/**
 * This API provides an implementation of immutable vectors, based on AVL trees.
 * 
 * The vectors being immutables, any functions that should modify a vector 
 * actually creates a new one, modifies it, and returns it.
 * 
 * For genericity reasons, the type of data are void*. It means that you can use
 * avl_vectors to store data of any type, but you'll have to explicitly  cast
 * the data you retrieve from the map. 
 * This API provides functions and typedef to use int and char* as data. You can
 * use other types, but you'll have to define them yourself.
 *
 * Because a common idiom when you won't require immutibality will be:
 *   avl_vector_t* tmp = avl_vector_push(vector, data);
 *   avl_vector_unref(vector);
 *   vector = tmp;
 * Every function can be called in a mutable way by using the _mutable suffixe.
 * This will do the same as the immutable version, but also unref the vector(s)
 * that are given as parameters.
 *
 * @example vector_main.c
 */


/** avl vectors will all have the type avl_vector_t */
typedef struct _avl_vector_t avl_vector_t;


#ifndef _INT_BOX_H
#define _INT_BOX_H
/** Boxing functions for integers. */
typedef int int_box_t;
/** returns a box holding an integer. */
int_box_t* make_int_box(int i);
/** returns the string representation of a boxed integer. */
char* int_box_as_string(void* data);
/** compares two integers. */
int compare_int_keys(void* key1, void* key2);
#endif

#ifndef _STRING_BOX_H
#define _STRING_BOX_H
/** Boxing functions for strings (ie. char* ). */
typedef char* string_box_t;
/** returns a box holding a string. */
string_box_t* make_string_box(char* str);
/** returns the string representation of a boxed string. */
char* string_box_as_string(void* data);
/** compares two strings. */
int compare_string_keys(void* key1, void* key2);
#endif


/**
 * Creates a new vector.
 *
 * @param  data_as_string  A pointer to the function that should be used to 
 *                          print the data stored in the array.
 *                          /!\ The string returned by data_as_string need to be
 *                             freeable. ie. free((*data_as_string)(data)) NEEDS
 *                             to work.
 * @return                 The newly created vector.
 */
avl_vector_t* avl_vector_create(char* (*data_as_string)(void* data));

/**
 * Get the size of the vector.
 *
 * @param  vec  The vector of which you want to get the size.
 * @return      The size of vector.
 */
int avl_vector_size (const avl_vector_t* vec);

/**
 * Update an element at a given index of a vector, and returns the new vector.
 * An mutable imperative notation of this function could be:
 *    vec[index] = data
 *
 * @param  vec    The vector to update.
 * @param  index  The index which should be updated.
 * @param  data   The new data for vec[index].
 * @return        The newly created vector.
 */
avl_vector_t* avl_vector_update(const avl_vector_t* vec, int index,
				void* data);
avl_vector_t* avl_vector_update_mutable(avl_vector_t* vec, int index,
					void* data);

/**
 * Get the element at a given index of a vector.
 * The imperative notation would be:
 *    vec[index]
 
 * @param  vec    The vector to look in.
 * @param  index  The index to look at.
 * @return        The element found at the index. The return value can be NULL.
 */
void* avl_vector_lookup(const avl_vector_t* vec, int index);

/**
 * Adds an element at the end of a vector.
 * A mutable imperative notation could be:
 *  - vec[size(tmp)+1] = data
 * 
 * @param  vec   The vector to update.
 * @param  data  The data to add in the vector.
 * @return       The newly created vector.
 */
avl_vector_t* avl_vector_push(const avl_vector_t* vec, void* data);
avl_vector_t* avl_vector_push_mutable(avl_vector_t* vec, void* data);

/**
 * Removes the last element from a vector.
 * 
 * @param[in]   vector  The vector you wish to remove the last element from.
 * @param[out]  data    The data that was removed from the vector. Note that
 *                       data can be NULL after function.
 * @return              The newly created vector.
 */
avl_vector_t* avl_vector_pop(const avl_vector_t* vec, void** data);
avl_vector_t* avl_vector_pop_mutable(avl_vector_t* vec, void** data);

/**
 * Merges two vectors.
 * 
 * @param  vec_front  The vector that will go in first position.
 * @param  vec_tail   The vector that will go in last position.
 * @return            The vector resulting from the concatenation of vec_front
 *                     and vec_tail.
 * */
avl_vector_t* avl_vector_merge(const avl_vector_t* vec_front,
			       const avl_vector_t* vec_tail);
avl_vector_t* avl_vector_merge_mutable(avl_vector_t* vec_front,
				       avl_vector_t* vec_tail);

/**
 * Splits a vector in two parts.
 *
 * @param[in]  vec_in    The vector that will be splitted.
 *                        vec_in can't be empty.
 * @param[in]  index     The index at which the split will be done.
 * @param[out] vec_out1  The first part of vec_in, up to index, included.
 * @param[out] vec_out2  The second (and last) part of vec_in.
 * @return               1 if the split was done. 0 otherwise.
 */
int avl_vector_split(const avl_vector_t* vec_in, int index,
		     avl_vector_t** vec_out1, avl_vector_t** vec_out2);
int avl_vector_split_mutable(avl_vector_t* vec_in, int index,
			     avl_vector_t** vec_out1, avl_vector_t** vec_out2);

/**
 * Destroys a vector. 
 * You can't just "free(vector)", but you need to call avl_vector_unref which
 * will do this for you, with respect for immutability and the internal
 * implementation of the structure.
 * 
 * @param  vec  The vector you wish to free.
 */
void avl_vector_unref(avl_vector_t* vec);

/**
 * Prints a vector to stdout. The format of the print is:
 * [ _, 1, _, 5 ] for a vector of int where _ represents empty cells.
 * The function data_as_string, that you provided when creating the vector
 * will be used to convert the data in the vector to string.
 *
 * @param  vec  The vector to print.
 */
void avl_vector_dump(const avl_vector_t* vec);



#endif
