#ifndef _FINGER_TYPES_
#define _FINGER_TYPES_

typedef int finger_data_t;

typedef enum {TREE_NODE, DATA_NODE} node_type_t;
typedef enum {EMPTY_NODE, SINGLE_NODE, DEEP_NODE} deep_type_t;
typedef enum {FINGER_LEFT, FINGER_RIGHT} side_t;

struct fingernode_t_def;

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

/* Finger node allocation and movement helpers */
fingernode_t* make_fingernode(int arity, node_type_t type);
fingernode_t* make_tree_node(int child_count, fingernode_t* children);
fingernode_t* copy_node(fingernode_t* node);
fingernode_t* make_treenode_and_cpy(int node_count, fingernode_t* new_node, fingernode_t** old_nodes, side_t side);
fingernode_t* split_append_treenode (fingernode_t* new_node, fingernode_t* old_nodes, fingernode_t** to_append, side_t side);
fingernode_t* make_data_node(int data_count, finger_data_t** data);
fingernode_t* make_datanode_and_cpy(int data_count, finger_data_t* new_data, finger_data_t** old_data, side_t side);
fingernode_t* split_append_datanode (finger_data_t* new_value, fingernode_t* old_values, fingernode_t** to_append, side_t side);

/* Deep allocation and creation helpers */
deep_t* make_deep();
deep_t* make_empty_node();
deep_t* make_single_node(fingernode_t* finger);
deep_t* make_deep_node(fingernode_t* left, deep_t* deeper, fingernode_t* right);

/* Node freeing */
void destroy_fingernode(fingernode_t* node);
void destroy_deep(deep_t* deep);

/* Node dereferencing */
int unref_fingernode(fingernode_t* node);
int unref_deep(deep_t* deep);

/* Tree content dump (~toList) */
void dump_finger(fingernode_t* node, void (*display)(finger_data_t*));
void dump_deep(deep_t* deep, void (*display)(finger_data_t*));

/* Value addition */
deep_t* append_node(deep_t* deep, fingernode_t* node, side_t side);
deep_t* append(deep_t* tree, finger_data_t* value, side_t side);

/* Index-based lookup */
finger_data_t* lookup_fingernodes(fingernode_t* node, int idx, int idx_cur);
finger_data_t* lookup(deep_t* tree, int idx);

/* Index-based update */
fingernode_t* update_fingernode(fingernode_t* node, int cur_idx, int idx, finger_data_t* new_value);
deep_t* update_up_to_depth (deep_t* tree, int depth, int cur_idx, int idx, side_t side, finger_data_t* new_value);
deep_t* update_deep(deep_t* tree, int idx, finger_data_t* new_value);

#endif
