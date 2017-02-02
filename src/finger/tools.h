#ifndef __TOOLS_H__
#define __TOOLS_H__

#define DEBUG 0

/**
 * Prints the string provided if debug mode enabled.
 * @param  fmt The string which must be printed.
 * @return     None.
 */
#define finger_debug(fmt)                  \
do {                                       \
    if (DEBUG) {                           \
        fprintf(stderr, fmt);            \
    }                                      \
 } while (0)

/**
 * Prints the string provided if debug mode enabled. Handle multiple args.
 * @param  fmt     The string which must be printed.
 * @param  VARARGS The other arguments.
 * @return         None.
 */
#define finger_args(fmt, ...)                  \
    do {                                       \
        if (DEBUG) {                           \
            fprintf(stderr, fmt, __VA_ARGS__); \
        }                                      \
    } while (0)

typedef int finger_data_t;

typedef enum {TREE_NODE, DATA_NODE} node_type_t;
typedef enum {EMPTY_NODE, SINGLE_NODE, DEEP_NODE} deep_type_t;
typedef enum {FINGER_LEFT, FINGER_RIGHT} side_t;

typedef struct fingernode_t_def{
  int ref_counter;
  int tag;
  int arity;
  int lookup_idx;
  node_type_t node_type;
  union {
    struct fingernode_t_def** children;
    finger_data_t** data;
  } content;
} fingernode_t;

typedef struct deep_t_def {
  deep_type_t deep_type;
  int ref_counter;
  int tag;
  fingernode_t* left;
  fingernode_t* right;
  union {
    fingernode_t* single;
    struct deep_t_def* deeper;
  } content;
} deep_t;

/**
 ** deep_t* simple linked list
 **/
typedef struct deep_list_t_def {
  deep_t* content;
  struct deep_list_t_def* next;
} deep_list_t;

deep_list_t* list_make();
int list_is_empty(deep_list_t* list);
void list_push(deep_t* deep, deep_list_t** list);
deep_t* list_pop(deep_list_t** list);
int list_size(deep_list_t* list);
void list_destroy(deep_list_t* list);

/**
 ** fingernode_t* double linked list
 **/
typedef struct finger_list_t_def {
  fingernode_t* content;
  struct finger_list_t_def* next;
  struct finger_list_t_def* prev;
} finger_list_t;

typedef struct finger_deque_t_def {
  int size;
  finger_list_t* first;
  finger_list_t* last;
} finger_deque_t;

finger_deque_t* deque_make();
int deque_is_empty(finger_deque_t* deque);
void deque_push_front(fingernode_t* val, finger_deque_t* deque);
void deque_push_back(fingernode_t* val, finger_deque_t* deque);
fingernode_t* deque_pop_first(finger_deque_t* deque);
fingernode_t* deque_pop_last(finger_deque_t* deque);
int deque_size(finger_deque_t* deque);
void deque_destroy(finger_deque_t* deque);

/**
 ** Miscellaneous
 **/

int finger_depth(fingernode_t* finger);
void dump_deep_debug(deep_t* deep, int span, void (*display)(finger_data_t**, int));
void dump_finger_debug(fingernode_t* node, int span, void (*display)(finger_data_t**, int));
#endif
