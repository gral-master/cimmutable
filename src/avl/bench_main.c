#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "avl_map.h"
#include "avl_vector.h"
#include "parser.h"

#define IMPLEM AVL
#define IMPLEM_NAME "avl"

double eval_vector_cmds(command** cmds, int size, avl_vector_t** vec) {
  struct timeval t1, t2;
  gettimeofday(&t1, NULL);
  
  for (int i = 0; i < size; i++) {
    command* cmd = cmds[i];
    int obj_in   = cmd->obj_in;
    int obj_in_2  = cmd->obj_in_2;
    int obj_out  = cmd->obj_out;
    int obj_out_2 = cmd->obj_out_2;
    switch (cmd->type) {
    case CREATE:
      vec[obj_out] = avl_vector_create(int_box_as_string);
      break;
    case UNREF:
      avl_vector_unref(vec[obj_in]);
      break;
    case UPDATE:
      vec[obj_out] = avl_vector_update(vec[obj_in], cmd->index,
				       make_int_box(cmd->data.as_int));
      break;
    case PUSH:
      vec[obj_out] = avl_vector_push(vec[obj_in],
				     make_int_box(cmd->data.as_int));
      break;
    case POP:
      ;void* data;
      vec[obj_out] = avl_vector_pop(vec[obj_in], &data);
      break;
    case MERGE:
      vec[obj_out] = avl_vector_merge(vec[obj_in], vec[obj_in_2]);
      break;
    case SPLIT:
      avl_vector_split(vec[obj_in], cmd->index, &vec[obj_out], &vec[obj_out_2]);
      break;
    case LOOKUP:
      avl_vector_lookup(vec[obj_in], cmd->index);
      break;
    case SIZE:
      avl_vector_size(vec[obj_in]); // hopefully won't be optimized out.
      break;
    case DUMP:
      avl_vector_dump(vec[obj_in]);
      break;
    }
  }

  gettimeofday(&t2, NULL);
  double elapsed_time = (t2.tv_sec - t1.tv_sec) * 1000.0;
  elapsed_time += (t2.tv_usec - t1.tv_usec) / 1000.0;
  return elapsed_time;
}


// Assuming int data for now.
double execute_vector (Prog* prog) {
  avl_vector_t** vec = malloc(prog->nb_var * sizeof(*vec));

  eval_vector_cmds(prog->init, prog->init_size, vec);
  return eval_vector_cmds(prog->bench, prog->bench_size, vec);
}

double execute_map (Prog* prog) {
  return (double)prog->init_size;
}


int main (int argc, char* argv[]) {

  if (argc <= 1) {
    fprintf(stderr, "Bench filename missing. Aborting.\n");
    exit (EXIT_FAILURE);
  }

  Prog* prog = read_file(argv[1]);

  if (! ( prog->implem & IMPLEM )) {
    fprintf(stderr, "Benchmark '%s' doesn't support %s. Aborting\n",
	    argv[1], IMPLEM_NAME);
    exit (EXIT_FAILURE);
  }

  double time;
  if (prog->struc == VECTOR) {
    time = execute_vector(prog);
  } else {
    time = execute_map(prog);
  }

  printf("Time elapsed: %f\n", time);

  return 0;
  
}
