#ifndef _AVL_MAP
#define _AVL_MAP

/**
 * This API provides an implementation of immutable maps, based on AVL trees.
 * 
 * The maps being immutables, any functions that should modify a map actually
 * creates a new one, modifies it, and returns it.
 *
 * The order of the keys in the map is unspecified.
 *
 * For genericity reasons, the type of the keys and data are void*. It means
 * that you can use anything as a key or a value, but you'll have to explicitly
 * cast the data you retrieve from the map. 
 * This API provides functions and typedef to use int and char* as either keys
 * or data. You can use other types, but you'll have to define them yourself.
 * 
 * Because a common idiom when you won't require immutibality will be:
 *   avl_map_t* tmp = avl_map_update(map, key, data);
 *   avl_map_unref(map);
 *   map = tmp;
 * Every function can be called in a mutable way by using the _mutable suffixe.
 * This will do the same as the immutable version, but also unref the map
 * that are given as parameters.
 *
 * @example map_main.c
 */

/** avl maps will all have the type avl_map_t */
typedef struct _avl_map_t avl_map_t;

/** iterators on avl map will have the type map_iterator_t */
typedef struct _map_iterator_t map_iterator_t;

/**********************
 * Boxing helpers 
 **********************/
/** Boxing functions for integers. */
typedef int int_box_t;
/** returns a box holding an integer. */
int_box_t* make_int_box(int i) __attribute__((weak));
/** returns the string representation of a boxed integer. */
char* int_box_as_string(void* data) __attribute__((weak));
/** compares two integers. */
int compare_int_keys(void* key1, void* key2) __attribute__((weak));

/** Boxing functions for strings (ie. char* ). */
typedef char* string_box_t;
/** returns a box holding a string. */
string_box_t* make_string_box(char* str) __attribute__((weak));
/** returns the string representation of a boxed string. */
char* string_box_as_string(void* data) __attribute__((weak));
/** compares two strings. */
int compare_string_keys(void* key1, void* key2) __attribute__((weak));


/**
 * Creates a new map.
 *
 * @param  key_as_string   A pointer to the function that should be used to 
 *                          print the keys of the map.
 *                          /!\ The string returned by key_as_string need to be
 *                             freeable. ie. free((*key_as_string)(key)) NEEDS
 *                             to work.
 * @param  data_as_string  A pointer to the function that should be used to 
 *                          print the data of the map.
 *                          /!\ The string returned by data_as_string need to be
 *                             freeable. ie. free((*data_as_string)(data)) NEEDS
 *                             to work.
 * @param  key_compare     A pointer to the function that should be used to
 *                          compare the keys.
 *                          This function should return 0, 1 or -1.
 *                          (like strcmp from <string.h> does)
 * @return                 The newly created map.
 */
avl_map_t* avl_map_create(char* (*key_as_string)(void*),
			  char* (*data_as_string)(void*),
			  int (*key_compare)(void*,void*));

/**
 * Get the size (ie. number of keys) of the map.
 *
 * @param  map  The vector of which you want to get the size.
 * @return      The size of map.
 */
int avl_map_size (const avl_map_t* map);

/**
 * Update an element at a given index of a map, and returns the new map.
 * An mutable imperative notation of this function could be:
 *    map{key} = data
 *
 * @param  map    The vector to update.
 * @param  key    The key which should be updated.
 * @param  data   The new data for map{keys}.
 * @return        The newly created map.
 */
avl_map_t* avl_map_update(const avl_map_t* map, void* key, void* data);
avl_map_t* avl_map_update_mutable(avl_map_t* map, void* key, void* data);

/**
 * Get the value associated to a key of a map.
 * The imperative notation would be:
 *    map{key}
 
 * @param  map  The vector to look in.
 * @param  key  The key to look at.
 * @return      The element associated to the key. The return value can be NULL.
 */
void* avl_map_lookup(const avl_map_t* map, void* key);

/**
 * Removes a key (and the value associated to it) from a map.
 * 
 * @param[in]  map   The map from which the key should be removed.
 * @param[in]  key   The key that should be removed.
 * @param[out] data  The data associated to the removed key.
 * @return           The newly created map.
 */
avl_map_t* avl_map_remove(const avl_map_t* map, void* key, void** data);
avl_map_t* avl_map_remove_mutable(avl_map_t* map, void* key, void** data);

/**
 * Returns the list (more precisely an array) of the keys of a map.
 * It doesn't return the size of the array. You can this information by
 * calling avl_map_size(map).
 * 
 * @param  map  The map from which you wish to get the keys.
 * @return      An array of the keys.
 */
void** avl_map_keys(const avl_map_t* map);

/**
 * Creates an iterator to iterate through the map keys/values.
 *
 * @param  map  The map on which the iterator will iterate.
 * @return      The newly created iterator
 */
map_iterator_t* avl_map_create_iterator(const avl_map_t* map);

/**
 * Iterates through a map thanks to a map_iterator_t object.
 * Note that the order in which the iterator goes through the map is undefined.
 * In particular, in most cases, it won't be the same as avl_map_keys.
 * Moreover, you'll always want to check the return value. A typical use is:
 *   void *key, *data;
 *   map_iterator_t* iter = avl_map_create_iterator(map);
 *   while ( avl_map_iterator_next(&iter, &key, &data) != 0 ) { 
 *     // do stuff with key and data.
 *   }
 * 
 * 
 * @param[in,out] iterator  The iterator created by avl_map_create_iterator.
 * @param[out]    key       The key of the next element in the map.
 * @param[in]     data      The data of the next element in the map.
 * @return                  0 if the iterator is at then end of the map,
 *                          1 otherwise.
 */
int avl_map_iterator_next(map_iterator_t** iterator,
			  void** key, void** data);

/**
 * Destroys a map.
 * You can't just "free(map)", but you need to call avl_map_unref which
 * will do this for you, with respect for immutability and the internal
 * implementation of the structure.
 * 
 * @param  map  The map you wish to free.
 */
void avl_map_unref(avl_map_t* map);

/**
 * Prints a map to stdout. The format of the print is:
 *  {
 *    key1  => data1,
 *    key2  => data2
 *  }
 * The functions data_as_string and key_as_string that you provided when 
 * creating the vector will be used to convert the data and keys of the map 
 * into strings.
 * 
 * The keys are left padded with spaces to all be printed on the same amount of 
 * characters. (it means that the "=>" are aligned.).
 * If you don't care about the alignement of the keys/data, you can use the faster
 * avl_map_dumpe_fast function.
 *
 * @param  map  The map to print.
 */
void avl_map_dump(const avl_map_t* map);

/**
 * Prints a map to stdout. See avl_map_dump documentation.
 *
 * @param  map  The map to print.
 */
void avl_map_dump_fast(const avl_map_t* map);

#endif
