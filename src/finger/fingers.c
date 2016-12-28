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
  tn->children = malloc(child_count * sizeof(tree_node_t*));
  memcpy(tn->children, children, child_count * sizeof(fingernode_t*));
  return res;
}

/**
 * Build a data node, which is a leaf node in a side finger.
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
  dn->data = malloc(data_count * sizeof(finger_data_t));
  memcpy(dn->data, data, (data_count * sizeof(finger_data_t)));
  return res;
}

/**
 * Build a deep node, which is a dorsal node of the finger tree
 */
deep_t* make_deep_node() {
  deep_t* res = malloc(sizeof(deep_t));
  res->ref_counter = 1;
  res->tag = 0;
  res->deeper = NULL;
  deep_node_t* deep_node = malloc(sizeof(deep_node_t));
  deep_node->left = NULL;
  deep_node->right = NULL;
  res->content.deep_node = deep_node;
  return res;
}

/**
 * Build a single node, which is (sometimes) the last dorsal node of
 * a finger tree
 */
deep_t* make_single_node(int data_count, finger_data_t* data) {
  deep_t* res = malloc(sizeof(deep_t));
  res->ref_counter = 1;
  res->tag = 0;
  res->deeper = NULL;
  single_node_t* single = malloc(sizeof(single_node_t));
  single->data = make_data_node(data_count, data)->content.data_node;
  res->content.single = single;
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
  switch (deep->deep_type) {
  case DEEP_NODE:
    free(deep->content.deep_node);
    break;
  case SINGLE_NODE:
    free(deep->content.single);
    break;
  default:
    break;
  }
  free(deep);
}

int unref_data_node(data_node_t* node) {
  node->ref_counter--;
  if (node->ref_counter) {
    return 1;
  }
  destroy_data_node(node);
  return 1;
}

int unref_tree_node(tree_node_t* node) {
  node->ref_counter--;
  if (node->ref_counter) {
    return 1;
  }
  for (int i=0; i<node->nb_children; i++) {
    unref_fingernode(node->children[i]);
  }
  destroy_tree_node(node);
  return 1;
}

int unref_fingernode(fingernode_t* node) {
  if (!node) {
    return 1;
  }
  switch(node->node_type) {
  case TREE_NODE:
    return unref_tree_node(node->content.tree_node);
    break;
  case DATA_NODE:
    return unref_data_node(node->content.data_node);
    break;
  default: /* This should not happen (famous last words ??) */
    fprintf(stderr, "UNKNOWN NODE TYPE");
    return 0;
  }
}

int unref_deep(deep_t* deep) {
  deep->ref_counter--;
  if (deep->ref_counter) {
    return 0;
  }
  deep_node_t* deep_node;
  data_node_t* data_node;
  switch (deep->deep_type) {
  case DEEP_NODE:
    deep_node = deep->content.deep_node;
    unref_fingernode(deep_node->left);
    unref_fingernode(deep_node->right);
    unref_deep(deep->deeper);
    destroy_deep_node(deep);
    break;
  case SINGLE_NODE:
    data_node = deep->content.single->data;
    unref_data_node(data_node);
    free(deep);
    break;
  }
  return 1;
}

void dump_data(data_node_t* node) {
  for (int i=0; i<node->nb_data; i++) {
    fprintf(stdout, "%d, ", node->data[i]);
  }
}

void dump_finger(fingernode_t* node) {
  if (!node) {
    return;
  }
  switch (node->node_type) {
  case TREE_NODE:
    for (int i=0; i<node->content.tree_node->nb_children; i++) {
      dump_finger(node->content.tree_node->children[i]);
    }
    break;
  case DATA_NODE:
    dump_data(node->content.data_node);
    break;
  default:
    break;
  }
}

void dump_deep(deep_t* deep) {
  deep_node_t* deep_node;
  data_node_t* data_node;
  while (deep) {
    switch (deep->deep_type) {
    case SINGLE_NODE:
      data_node = deep->content.single->data;
      dump_data(data_node);
      break;
    case DEEP_NODE:
      deep_node = deep->content.deep_node;
      dump_finger(deep_node->left);
      dump_finger(deep_node->right);
      break;
    default:
      break;
    }
    deep = deep->deeper;
  }
}

deep_t* append(deep_t* tree, finger_data_t* value) {
  return NULL;
}

deep_t* prepend(deep_t* tree, finger_data_t* value) {
  return NULL;
}
fingernode_t* node_push(fingernode_t* node, fingernode_t* value) {
  return NULL;
}

int main(int argc, char** argv) {
  printf("Work in progress...\n");
}
