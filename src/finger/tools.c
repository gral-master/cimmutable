#include <stdlib.h>
#include <stdio.h>

#include "tools.h"
#include "fingers.h"

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
 ** LIST OPERATIONS
 **/

deep_list_t* list_make() {
  return malloc(sizeof(deep_list_t));
}

int list_is_empty(deep_list_t* list) {
  return list == NULL;
}

void list_push(deep_t* deep, deep_list_t** list) {
  deep_list_t* new_head = malloc(sizeof(deep_list_t));
  new_head->content = deep;
  new_head->next = *list;
  *list = new_head;
}

deep_t* list_pop(deep_list_t** list) {
  deep_t* res = (*list)->content;
  *list = (*list)->next;
  return res;
}

int list_size(deep_list_t* list) {
  int i;
  for (i=0; list; list=list->next, i++);
  return i;
}

void list_destroy(deep_list_t* list) {
  deep_list_t* tmp;
  while (list) {
    tmp = list->next;
    free(list);
    list = tmp;
  }
}

/**
 ** DEQUE OPERATIONS
 **/

finger_deque_t* deque_make() {
  return malloc(sizeof(finger_deque_t));
}

int deque_is_empty(finger_deque_t* deque) {
  return deque->first == NULL || deque->last == NULL;
}

void deque_push_front(fingernode_t* val, finger_deque_t* deque) {
  finger_list_t* el = malloc(sizeof(deep_list_t));
  el->content = val;
  el->next = deque->first;
  el->prev = NULL;
  deque->first = el;
  deque->size++;
}

void deque_push_back(fingernode_t* val, finger_deque_t* deque) {
  finger_list_t* el = malloc(sizeof(deep_list_t));
  el->content = val;
  el->next = NULL;
  el->prev = deque->first;
  deque->last = el;
  deque->size++;
}

fingernode_t* deque_pop_first(finger_deque_t* deque) {
  finger_list_t* first = deque->first;
  fingernode_t* res = first->content;
  deque->first = first->next;
  free(first);
  deque->size--;
  return res;
}

fingernode_t* deque_pop_last(finger_deque_t* deque) {
  finger_list_t* last = deque->last;
  fingernode_t* res = last->content;
  deque->last = last->prev;
  free(last);
  deque->size--;
  return res;
}

int deque_size(finger_deque_t* deque) {
  return deque->size;
}

void deque_destroy(finger_deque_t* deque) {
  finger_list_t* tmp;
  finger_list_t* cur = deque->first;
  while(cur) {
    tmp = cur->next;
    free(cur);
    cur = tmp;
  }
  free(deque);
}
