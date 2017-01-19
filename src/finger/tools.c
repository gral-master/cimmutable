#include <stdlib.h>
#include <stdio.h>

#include "tools.h"
#include "types.h"

/**
 **  DOT TOOLS
 **/
void tag_nodes(fingernode_t* node, int* tag) {
  node->tag = (*tag)++;
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

/**
 ** STACK OPERATIONS
 **/

int stack_is_empty(deep_stack_t* stack) {
  return stack == NULL;
}

void stack_push(deep_t* deep, deep_stack_t** stack) {
  deep_stack_t* new_head = malloc(sizeof(deep_stack_t));
  new_head->content = deep;
  new_head->next = *stack;
  *stack = new_head;
}

deep_t* stack_pop(deep_stack_t** stack) {
  deep_t* res = (*stack)->content;
  *stack = (*stack)->next;
  return res;
}

int stack_size(deep_stack_t* stack) {
  int i;
  for (i=0; stack; stack=stack->next, i++);
  return i;
}

void stack_destroy(deep_stack_t* stack) {
  while (stack) {
    deep_stack_t* tmp = stack->next;
    free(stack);
    stack = tmp;
  }
}
