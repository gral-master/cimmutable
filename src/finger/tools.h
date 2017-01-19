#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "types.h"
#include "tools.h"

typedef struct deep_stack_t_def {
  deep_t* content;
  struct deep_stack_t_def* next;
} deep_stack_t;

void tag_deeps(deep_t*);
void dump_to_dot(deep_t* tree, char* fname);

int stack_is_empty(deep_stack_t* stack);
void stack_push(deep_t* deep, deep_stack_t** stack);
deep_t* stack_pop(deep_stack_t** stack);
int stack_size(deep_stack_t* stack);
void stack_destroy(deep_stack_t* stack);

#endif
