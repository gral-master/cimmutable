#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "types.h"

#define NODE_MAX_SIZE 4

//#define FINGER_DEBUG

#ifdef FINGER_DEBUG
#define finger_debug(str) fprintf(stdout, "%s\n", str);
#else
#define finger_debug(str) ;
#endif

/**
 * Return blank finger node with ref counter properly set
 */
fingernode_t* make_fingernode(int arity, node_type_t type) {
  finger_debug("make_fingernode");
  fingernode_t* res = malloc(sizeof(fingernode_t));
  res->ref_counter = 1;
  res->arity = arity;
  res->node_type = type;
  switch (type) {
  case DATA_NODE:
    res->content.data = malloc(arity * sizeof(finger_data_t));
    break;
  case TREE_NODE:
    res->content.children = malloc(arity * sizeof(fingernode_t*));
    break;
  default:
    break;
  }
  return res;
}

/**
 * Build a tree node from original tree nodes and new node
 * Guaranteed to be called with legit params.
 * ASSERT: depth of new_node is equal to depth of each node in nodes
 */
fingernode_t* make_treenode_and_cpyl(int node_count, fingernode_t* new_node, fingernode_t** old_nodes) {
  finger_debug("make_treenode_and_cpyl");
  fingernode_t* res = make_fingernode(node_count + 1, TREE_NODE);
  res->content.children[0] = new_node;
  new_node->ref_counter++;
  finger_debug("loop");
  for (int i=1; i<node_count+1; i++) {
    res->content.children[i] = old_nodes[i-1];
    res->content.children[i]->ref_counter++;
  }
  return res;
}

/**
 * Build a data node from original data nodes and new value
 */
fingernode_t* make_datanode_and_cpyl(int data_count, finger_data_t new_data, finger_data_t* old_data) {
  finger_debug("make_datanode_and_cpyl");
  fingernode_t* res = make_fingernode(data_count + 1, DATA_NODE);
  res->content.data[0] = new_data;
  for (int i=1; i<data_count+1; i++) {
    res->content.data[i] = old_data[i-1];
  }
  return res;
}

/**
 * Return blank deep node with ref counter properly set
 */
deep_t* make_deep() {
  finger_debug("make_deep");
  deep_t* res = malloc(sizeof(deep_t));
  res->ref_counter = 1;
  res->tag = 0;
  return res;
}

/**
 * Build an empty node. This can be simplified to a global, indelible empty node variable
 * since we can reference the same one multiple times without impacting the tree's semantics.
 */
deep_t* make_empty_node() {
  finger_debug("make_empty_node");
  deep_t* res = make_deep();
  res->deep_type = EMPTY_NODE;
  return res;
}

/**
 * Build a single node from the fingernode it is supposed to contain
 */
deep_t* make_single_node(fingernode_t* finger) {
  finger_debug("make_single_node");
  deep_t* res = make_deep();
  res->deep_type = SINGLE_NODE;
  res->content.single = finger;
  return res;
}

/**
 * Build a deep node, which is a dorsal node of the finger tree
 */
deep_t* make_deep_node(fingernode_t* left, deep_t* deeper, fingernode_t* right) {
  finger_debug("make_deep_node");
  deep_t* res = make_deep();
  res->deep_type = DEEP_NODE;
  res->left = left;
  res->content.deeper = deeper;
  res->right = right;
  return res;
}

/**
 * Destroy a fingernode, no recursion
 */
void destroy_fingernode(fingernode_t* node) {
  finger_debug("destroy_fingernode");
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

/**
 * Destroy a deep, no recursion
 */
void destroy_deep(deep_t* deep) {
  finger_debug("destroy_deep");
  free(deep);
}

/**
 * Recursively update reference counts in node to have one less reference
 */
int unref_fingernode(fingernode_t* node) {
  finger_debug("unref_fingernode");
  node->ref_counter--;
  if (node->ref_counter) { // i.e. ref_count != 0
    return 1;
  }
  // recursive unref
  if (node->node_type == TREE_NODE) {
    fingernode_t** children = node->content.children;
    for (int i=0; i<node->arity; i++) {
      unref_fingernode(children[i]);
    }
  }
  destroy_fingernode(node);
  return 1;
}

/**
 * Recursively update reference counts in deep to have one less reference
 */
int unref_deep(deep_t* deep) {
  finger_debug("unref_deep");
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
  return 1;
}

/**
 * Using the display function for the finger data type, recursively dump node
 */
void dump_finger(fingernode_t* node, void (*display)(finger_data_t)) {
  finger_debug("dump_finger");
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

/**
 * Using the display function for the finger data type, recursively dump deep
 */
void dump_deep(deep_t* deep, void (*display)(finger_data_t)) {
  finger_debug("dump_deep");
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
 * Given a finger node, prepend it in the tree rooted in deep
 */
deep_t* prepend_node(deep_t* deep, fingernode_t* node) {
  finger_debug("prepend_node");

  if (deep->deep_type == DEEP_NODE) {
    deep_t* newdeep = malloc(sizeof(deep_t));
    switch (deep->deep_type) {
    case DEEP_NODE:
      newdeep->deep_type = DEEP_NODE;
      break;
    case SINGLE_NODE:
      newdeep->deep_type = DEEP_NODE;
      break;
    case EMPTY_NODE:
      newdeep->deep_type = SINGLE_NODE;
      break;
    default:
      break;
    }

    fingernode_t* left = deep->left;
    if (left->arity == NODE_MAX_SIZE) {

      // Do the changes at the root node
      fingernode_t* newleft = make_fingernode(2, left->node_type);
      newleft->content.children[0] = node;
      newleft->content.children[1] = left->content.children[0];

      fingernode_t* append_child = make_fingernode(3, left->node_type);
      memcpy(append_child->content.children[0], left->content.children[1], 3 * sizeof(fingernode_t*));
      fingernode_t* append = malloc(sizeof(fingernode_t));
      append->node_type = TREE_NODE; // Higher order node, so tree node only
      append->arity = 1;
      append->content.children = malloc(sizeof(fingernode_t*));
      append->content.children[0] = append_child;

      newdeep->left = newleft;
      newdeep->right = deep->right;
      if (newdeep->right) {
        newdeep->right->ref_counter++;
      }

      // Recur on deeper
      newdeep->content.deeper = prepend_node(newdeep->content.deeper, append);
    }
    else {
      newdeep->content.deeper = deep->content.deeper;
      newdeep->right = deep->right;
      newdeep->content.deeper->ref_counter++;
      newdeep->right->ref_counter++;
      newdeep->left = make_treenode_and_cpyl(left->arity, node, left->content.children);
    }
    return newdeep;
  }

  if (deep->deep_type == SINGLE_NODE) {
    fingernode_t* single = deep->content.single;
    if (single->arity == NODE_MAX_SIZE) {
      single->ref_counter++;
      return make_deep_node(node, make_empty_node(), deep->content.single);
    }
    else {
      return make_single_node(make_treenode_and_cpyl(single->arity, node, single->content.children));
    }
  }

  if (deep->deep_type == EMPTY_NODE) {
    return make_single_node(node);
  }
  return NULL;
}

/**
 * Given a value, attempt to prepend it to the tree rooted in tree,
 * and append it recursively as a node if this is not possible
 */
deep_t* prepend(deep_t* tree, finger_data_t value) {
  finger_debug("prepend");

  if (tree->deep_type == DEEP_NODE) {
    deep_t* newdeep = malloc(sizeof(deep_t));
    switch (tree->deep_type) {
    case DEEP_NODE:
      newdeep->deep_type = DEEP_NODE;
      break;
    case SINGLE_NODE:
      newdeep->deep_type = DEEP_NODE;
      break;
    case EMPTY_NODE:
      newdeep->deep_type = SINGLE_NODE;
      break;
    default:
      break;
    }

    fingernode_t* left = tree->left;
    if (left->arity == NODE_MAX_SIZE) {
      // Do the changes at the root node
      fingernode_t* newleft = make_fingernode(2, left->node_type);
      newleft->content.data[0] = value;
      newleft->content.data[1] = left->content.data[0];

      fingernode_t* append_child = make_fingernode(3, left->node_type);
      memcpy(append_child->content.children[0], left->content.children[1], 3 * sizeof(fingernode_t*));
      fingernode_t* append = make_fingernode(1, TREE_NODE);
      append->content.children[0] = append_child;

      newdeep->left = newleft;
      newdeep->right = tree->right;
      if (newdeep->right) {
        newdeep->right->ref_counter++;
      }

      // Deeper recur
      newdeep->content.deeper = prepend_node(newdeep->content.deeper, append);
    }
    else {
      newdeep->content.deeper = tree->content.deeper;
      newdeep->right = tree->right;
      newdeep->content.deeper->ref_counter++;
      newdeep->right->ref_counter++;
      newdeep->left = make_datanode_and_cpyl(left->arity, value, left->content.data);
    }
    return newdeep;
  }

  fingernode_t* valuenode = make_fingernode(1, DATA_NODE);
  valuenode->content.data[0] = value;
  if (tree->deep_type == SINGLE_NODE) {
    fingernode_t* single = tree->content.single;
    if (single->arity >= NODE_MAX_SIZE) {
      single->ref_counter++;
      return make_deep_node(valuenode, make_empty_node(), single);
    }
    else {
      fingernode_t* t = make_datanode_and_cpyl(single->arity, value, single->content.data);
      return make_single_node(t);
    }
  }

  if (tree->deep_type == EMPTY_NODE) {
    return make_single_node(valuenode);
  }
  return NULL;
}

/**
 **  DOT TOOLS
 **/
void tag_nodes(fingernode_t* node, int* tag) {
  node->tag = *tag;
  *tag = *tag + 1;
  if (node->node_type == TREE_NODE) {
    for (int i=0; i<node->arity; i++) {
      tag_nodes(node->content.children[i], tag);
    }
  }
}

void tag_deeps(deep_t* tree) {
  int tag = 0;
  while (tree->deep_type == DEEP_NODE) {
    tag_nodes(tree->left, &tag);
    tree->tag = tag++;
    tag_nodes(tree->right, &tag);
    tree = tree->content.deeper;
  }
  if (tree->deep_type == SINGLE_NODE) {
    tree->tag = tag++;
    tag_nodes(tree->content.single, &tag);
  }
  if (tree->deep_type == EMPTY_NODE) {
    tree->tag = tag++;
  }
}

void fingernode_to_dot(fingernode_t* node, FILE* stream) {
  int tag = node->tag;
  if (node->node_type == TREE_NODE) {
    for (int i=0; i<node->arity; i++) {
      fprintf(stream, "%d -> %d;\n", tag, node->content.children[i]->tag);
      fingernode_to_dot(node->content.children[i], stream);
    }
  }
}

void deeps_to_dot(deep_t* tree, FILE* stream) {
  int tag = tree->tag;
  switch(tree->deep_type) {
  case DEEP_NODE:
    fprintf(stream, "%d -> %d;\n", tag, tree->content.deeper->tag);
    deeps_to_dot(tree->content.deeper, stream);
    fprintf(stream, "%d -> %d;\n", tag, tree->left->tag);
    fingernode_to_dot(tree->left, stream);
    fprintf(stream, "%d -> %d;\n", tag, tree->right->tag);
    fingernode_to_dot(tree->right, stream);
    break;
  case SINGLE_NODE:
    fprintf(stream, "%d -> %d;\n", tag, tree->content.single->tag);
    fingernode_to_dot(tree->content.single, stream);
    break;
  case EMPTY_NODE:
    break;
  }
}

void dump_to_dot(deep_t* tree, char* fname) {
  FILE* f;
  if ((f = fopen(fname, "w")) == NULL) {
    fprintf(stderr, "error opening file\n");
  }
  fprintf(f, "digraph tree {\n");
  deeps_to_dot(tree, f);
  fprintf(f, "}");
}

deep_t* append(deep_t* tree, finger_data_t* value) {
  return NULL;
}

void display(finger_data_t data) {
  fprintf(stdout, "%d", data);
}

int main(int argc, char** argv) {
  finger_debug("empty");
  deep_t* tree = make_empty_node();
  dump_to_dot(tree, "empty.dot");

  finger_debug("add 2");
  deep_t* t1 = prepend(tree, 2);
  dump_to_dot(t1, "t1.dot");

  finger_debug("add 3");
  deep_t* t2 = prepend(t1, 3);
  dump_to_dot(t2, "t2.dot");

  finger_debug("add 4");
  deep_t* t3 = prepend(t2, 4);
  dump_to_dot(t3, "t3.dot");

  finger_debug("add 5");
  deep_t* t4 = prepend(t3, 5);
  dump_to_dot(t4, "t4.dot");

  finger_debug("add 6");
  deep_t* t5 = prepend(t4, 6);
  dump_to_dot(t5, "t5.dot");

  dump_deep(t5, &display);

  finger_debug("unref");
  unref_deep(t5);
}
