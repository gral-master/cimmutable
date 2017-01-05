#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "types.h"

#define CHILDREN_MAX_SIZE 3
#define DATA_MAX_SIZE 4

#define FINGER_DEBUG

#ifdef FINGER_DEBUG
#define finger_debug(str) fprintf("%s\n", str);
#else
#define finger_debug(str) ;
#endif

/**
 * Build a tree node from original tree nodes and new node
 * Guaranteed to be called with legit params.
 * ASSERT: depth of new_node is equal to depth of each node in nodes
 */
fingernode_t* make_treenode_and_cpy(int node_count, fingernode_t* new_node, fingernode_t** old_nodes) {
  fingernode_t* res = malloc(sizeof(fingernode_t));
  res->node_type = TREE_NODE;
  res->ref_counter = 1;
  res->arity = node_count + 1;
  res->content.children[0] = new_node;
  if (old_nodes) {
    memcpy(res->content.children[1], old_nodes, node_count * sizeof(fingernode_t*));
    for (int i=1; i<node_count+1; i++) {       // new refs on the old nodes
      res->content.children[i]->ref_counter++;
    }
  }
  return res;
}

/**
 * TODO: carefully work out wtf to do with finger_data_t (prolly **, actually)
 */
fingernode_t* make_datanode_and_cpy(int data_count, finger_data_t* new_data, finger_data_t* old_data) {
  fingernode_t* res = malloc(sizeof(fingernode_t));
  res->node_type = DATA_NODE;
  res->ref_counter = 1;
  res->arity = data_count + 1;
  res->content.children[0] = new_data;
  if (old_data) {
    memcpy(res->content.children[1], old_data, data_count * sizeof(fingernode_t*));
  }
  return res;
}

/**
 * Build an empty node. This can be simplified to a global, indelible empty node variable
 * since we can reference the same one multiple times without impacting the tree's semantics.
 */
deep_t* make_empty_node() {
  deep_t* res = malloc(sizeof(deep_t));
  res->deep_type = EMPTY_NODE;
  return res;
}

/**
 * Build a single node, which is (sometimes) the last dorsal node of
 * a finger tree
 */
deep_t* make_single_node(int data_count, fingernode_t* single) {
  deep_t* res = malloc(sizeof(deep_t));
  res->deep_type = SINGLE_NODE;
  res->ref_counter = 1;
  res->tag = 0;
  res->content.single = single;
  return res;
}

/**
 * Build a deep node, which is a dorsal node of the finger tree
 */
deep_t* make_deep_node(fingernode_t* left, deep_t* deeper, fingernode_t* right) {
  deep_t* res = malloc(sizeof(deep_t));
  res->deep_type = DEEP_NODE;
  res->ref_counter = 1;
  res->tag = 0; 
  res->left = left;
  res->content.deeper = deeper;
  res->right = right;
  return res;
}

void destroy_fingernode(fingernode_t* node) {
  switch (node->node_type) {
  case TREE_NODE:
    free(node->content.children);
    break;
  case DATA_NODE:
    free(node->content.data);
    break;
  default:
    break;
  }
  free(node);
}

void destroy_deep(deep_t* deep) {
  free(deep);
}

int unref_fingernode(fingernode_t* node) {
  node->ref_counter--;
  if (node->ref_counter) { // i.e. ref_count != 0
    return 1;
  }
  fingernode_t** children = node->content.children;
  for (int i=0; i<node->arity; i++) {
    unref_fingernode(children[i]);
  }
  destroy_fingernode(node);
  return 1;
}

int unref_deep(deep_t* deep) {
  deep->ref_counter--;
  if (deep->ref_counter) {
    return 0;
  }
  switch (deep->deep_type) {
  case DEEP_NODE:
    unref_fingernode(deep->left);
    unref_deep(deep->content.deeper);
    unref_fingernode(deep->right);
  case SINGLE_NODE:
    unref_fingernode(deep->content.single);
    break;
  case EMPTY_NODE:
  default:
    break;
  }
  destroy_deep(deep);
}

void dump_finger(fingernode_t* node, void (*display)(finger_data_t)) {
  switch (node->node_type) {
  case TREE_NODE:
    for (int i=0; i<node->arity; i++) {
      dump_finger(node->content.children[i], display);
    }
    break;
  case DATA_NODE:
    for (int i=0; i<node->arity; i++) {
      display(node->content.data[i]);
    }
  default:
    break;
  }
}

void dump_deep(deep_t* deep, void (*display)(finger_data_t)) {
  switch (deep->deep_type) {
  case DEEP_NODE:
    dump_finger(deep->left, display);
    dump_deep(deep->content.deeper, display);
    dump_finger(deep->right, display);
    break;
  case SINGLE_NODE:
    dump_finger(deep->content.single, display);
    break;
  case EMPTY_NODE:
  default:
    break;
  }
}

/**
 * Type match guaranteed by construction
 * Called only if the deep's type is DEEP_NODE
 * We never prepend on SINGLE_NODE
 * We never even touch an EMPTY_NODE
 */
void prepend_node(deep_t* deep, fingernode_t* node) {
  // Whatever happens, if we're here we're modifying deep->left
  deep->content.deeper->ref_counter++;
  deep->right->ref_counter++;

  fingernode_t* left = deep->left;
  if (left->arity == 4) {
    // Do the changes at the root node
    // Recur on deeper
  }
  else {
    return make_treenode_and_cpy(left->arity, node, left->content.children);
  }
}

/**
 * WiP
 */
deep_t* prepend(deep_t* tree, finger_data_t* value) {
  switch (tree->deep_type) {
  case EMPTY_NODE:
    return make_single_from_data(value);
  case SINGLE_NODE:
    return make_deep(NULL, NULL, NULL);
  case DEEP_NODE:
    return make_deep(NULL, NULL, NULL);
  default:
    break;
  }
  return NULL;
}

deep_t* append(deep_t* tree, finger_data_t* value) {
  return NULL;
}
fingernode_t* node_push(fingernode_t* node, fingernode_t* value) {
  return NULL;
}

int main(int argc, char** argv) {
  printf("Work in progress...\n");
}
