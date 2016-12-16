#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "types.h"

#define CHILDREN_MAX_SIZE 3
#define DATA_MAX_SIZE 4

/**
 * Build a tree node, which is an internal node in a side finger.
 * Children are assumed unsafe and are binary-copied from input.
 * The reference to the node is assumed kept from this call (ref_counter is 1)
 */
fingernode_t* make_tree_node(int child_count, fingernode_t* children) {
  fingernode_t* res = malloc(sizeof(fingernode_t));
  res->node_type = TREE_NODE;
  tree_node_t* tn = res->content.tree_node;
  tn->ref_counter = 1;
  tn->tag = 0;
  tn->nb_children = 0;
  tn->children = malloc(child_count * sizeof(tree_node_t));
  memcpy(tn->children, children, child_count * sizeof(fingernode_t));
  return res;
}

/**
 * Build a data node, which is an internal node in a side finger.
 * Data are assumed unsafe and are binary-copied from input.
 * The reference to the node is assumed kept from this call (ref_counter is 1)
 */
fingernode_t* make_data_node(int data_count, finger_data_t* data) {
  fingernode_t* res = malloc(sizeof(data_node_t));
  res->node_type = DATA_NODE;
  data_node_t* dn = res->content.data_node;
  dn->ref_counter = 1;
  dn->tag = 0;
  dn->nb_data = data_count;
  dn->data = malloc(data_count * sizeof(data_node_t));
  memcpy(dn->data, data, (data_count * sizeof(finger_data_t)));
  return res;
}

/**
 * Build a deep node, which is a dorsal node of the finger tree
 */
deep_t* make_deep() {
  deep_t* res = malloc(sizeof(deep_t));
  res->ref_counter = 1;
  res->tag = 0;
  res->deep_child = NULL;
  res->left = NULL;
  res->right = NULL;
  return res;
}

void destroy_tree_node(tree_node_t* node) {
  free(node->children);
  free(node);
}

void destroy_data_node(data_node_t* node) {
  free(node->data);
  free(node);
}

void destroy_deep_node(deep_t* deep) {
  // Needs more logic?
  free(deep);
}

deep_t* push(deep_t* tree, finger_data_t* value) {
  return NULL;
}

fingernode_t* node_push(fingernode_t* node, fingernode_t* value) {
  return NULL;
}

int main(int argc, char** argv) {
  printf("Work in progress...\n");
}
