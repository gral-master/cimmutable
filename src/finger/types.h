#ifndef _FINGER_TYPES_
#define _FINGER_TYPES_

typedef int finger_data_t;

typedef enum {TREE_NODE, DATA_NODE} node_type_t;
typedef enum {EMPTY_NODE, SINGLE_NODE, DEEP_NODE} deep_type_t;
typedef enum {LEFT, RIGHT} side_t;

struct fingernode_t_def;

typedef struct fingernode_t_def{
  int ref_counter;
  int tag;
  int arity;
  int lookup_idx;
  node_type_t node_type;
  union {
    struct fingernode_t_def** children;
    finger_data_t* data;
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

/* Node creation */
fingernode_t* make_tree_node(int child_count, fingernode_t* children);
fingernode_t* make_data_node(int data_count, finger_data_t* data);
deep_t* make_deep();

/* Node dereferencing */
int unref_fingernode(fingernode_t* node);
int unref_deep(deep_t* deep);

/* Utilities */
void dump_deep(deep_t* deep, void (*display)(finger_data_t));

#endif
