#ifndef _AVL_VECTOR
#define _AVL_VECTOR

typedef struct _avl_vector_t avl_vector_t;


typedef struct _vector_data_t vector_data_t;
char* data_as_string(vector_data_t* data);



avl_vector_t* avl_vector_create();

int avl_vector_size (avl_vector_t* vec);

avl_vector_t* avl_vector_update(avl_vector_t* vec, int index,
				vector_data_t* data);


vector_data_t* avl_vector_lookup(avl_vector_t* vec, int index);




void avl_vector_dump(avl_vector_t* vec);



#endif
