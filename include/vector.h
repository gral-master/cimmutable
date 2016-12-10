#ifndef _VECTOR_H
#define _VECTOR_H

/**********************************************
████████ ██    ██ ██████  ███████ ███████
   ██     ██  ██  ██   ██ ██      ██
   ██      ████   ██████  █████   ███████
   ██       ██    ██      ██           ██
   ██       ██    ██      ███████ ███████
***********************************************/

/** Data used internaly. */
typedef void* imc_data_t;

/** Each structure is defined as is :
 *  [Structure] {
 *      self#update
 *      self#lookup
 *      self#push
 *      self#pop
 *      self#merge
 *      self#split
 *      self#size
 *      self#unref
 *      self#dump
 *  }
 *  For each function, first variable is always self, i.e., var->size(var);
 */

/** Manipulation functions. */
#define __STRUCTURES_FUNCTIONS__ \
    struct _vector* (*update)(struct _vector*, imc_data_t*, int); \
    imc_data_t*     (*lookup)(struct _vector*, int); \
    struct _vector* (*push)  (struct _vector*, imc_data_t*); \
    struct _vector* (*pop)   (struct _vector*, imc_data_t*); \
    struct _vector* (*merge) (struct _vector*, struct _vector*); \
    int (*split)(struct _vector*, int, struct _vector**, struct _vector**); \
    int (*size) (struct _vector*); \
    int (*unref)(struct _vector* vec); \
    void (*dump)(struct _vector* vec);

/** Public structure. */
typedef struct _vector {
	__STRUCTURES_FUNCTIONS__
} imc_vector_t;

/** Internal representation of avl. */
typedef struct {
    __STRUCTURES_FUNCTIONS__
    struct _avl* entry;
} imc_avl_t;

/** Internal representation of finger tree. */
typedef struct {
    __STRUCTURES_FUNCTIONS__
    struct _finger* entry;
} imc_finger_t;

/** Internal representation of RRB Vector. */
typedef struct {
    __STRUCTURES_FUNCTIONS__
    struct _rrb* entry;
} imc_rrb_t;

/**********************************************
███████ ██    ██ ███    ██  ██████ ███████
██      ██    ██ ████   ██ ██      ██
█████   ██    ██ ██ ██  ██ ██      ███████
██      ██    ██ ██  ██ ██ ██           ██
██       ██████  ██   ████  ██████ ███████
***********************************************/

// imc_vector_t* imc_vector_create();

/** Creates corresponding structures. */
imc_vector_t* imc_avl_new();
imc_vector_t* imc_finger_new();
imc_vector_t* imc_rrb_new();

#endif
