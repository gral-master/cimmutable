#ifndef _FINGER_TYPES_
#define _FINGER_TYPES_

typedef int finger_data_t;
typedef enum {TREE_NODE, DATA_NODE} node_type_t;

struct fingernode_t_def;

typedef struct {
  int ref_counter;
  int tag;
  int nb_data;
  struct fingernode_t_def** data;
} data_node_t;

typedef struct {
  int ref_counter;
  int tag;
  int nb_children;
  struct fingernode_t_def** children;
} tree_node_t;

typedef struct fingernode_t_def{
  node_type_t node_type;
  union {
    tree_node_t* tree_node;
    data_node_t* data_node;
  } content;
} fingernode_t;

typedef struct deep_t_def {
  int ref_counter;
  int tag;
  struct deep_t_def* deep_child;
  fingernode_t* left;
  fingernode_t* right;
} deep_t;

/* Node creation */
fingernode_t* make_tree_node(int child_count, fingernode_t* children);
fingernode_t* make_data_node(int data_count, finger_data_t* data);
deep_t* make_deep();

/* Node dereferencing */
int unref_fingernode(fingernode_t* node);
int unref_deep(deep_t* deep);

#endif
