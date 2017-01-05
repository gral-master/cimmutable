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
 * Build a tree node, which is an internal node in a side finger.
 * Children are assumed unsafe and are binary-copied from input.
 * The reference to the node is assumed kept from this call (ref_counter is 1)
 */
fingernode_t* make_tree_node(int child_count, fingernode_t* children) {
  /*
  fingernode_t* res = malloc(sizeof(fingernode_t));
  res->node_type = TREE_NODE;
  tree_node_t* tn = res->content.tree_node;
  tn->ref_counter = 1;
  tn->tag = 0;
  tn->nb_children = 0;
  tn->children = malloc(child_count * sizeof(tree_node_t*));
  memcpy(tn->children, children, child_count * sizeof(fingernode_t*));
  return res;
*/
  return NULL;
}

/**
 * Build a data node, which is a leaf node in a side finger.
 * Data are assumed unsafe and are binary-copied from input.
 * The reference to the node is assumed kept from this call (ref_counter is 1)
 */
fingernode_t* make_data_node(int data_count, finger_data_t* data) {
/*
  fingernode_t* res = malloc(sizeof(data_node_t));
  res->node_type = DATA_NODE;
  data_node_t* dn = res->content.data_node;
  dn->ref_counter = 1;
  dn->tag = 0;
  dn->nb_data = data_count;
  dn->data = malloc(data_count * sizeof(finger_data_t));
  memcpy(dn->data, data, (data_count * sizeof(finger_data_t)));
  return res;
*/
  return NULL;
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

deep_t* prepend(deep_t* tree, finger_data_t* value) {
  switch (tree->deep_type) {
  case EMPTY_NODE:
    return make_single(1, value);
  case SINGLE_NODE:
    return make_deep_node(NULL, NULL, NULL);
  case DEEP_NODE:
    return make_deep_node(NULL, NULL, NULL);
    break;
  default:
    return NULL;
  }
}

void prepend_node(deep_t* tree, fingernode_t* node) {
  return;
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
