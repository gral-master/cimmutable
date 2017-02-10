#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <getopt.h>

#include "rrb_vector.h"
#include "rrb_dumper.h"
#include "parser.h"

// IMPLEM : AVL or RRB or FINGER
#define IMPLEM AVL
// IMPLEM_NAME : doesn't matter, only here for the prints.
#define IMPLEM_NAME "avl"

int is_test = 0, is_bench = 0;

double eval_vector_cmds(command** cmds, int size, rrb_t** vec) {
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

    for (int i = 0; i < size; i++) {
        command* cmd = cmds[i];
        int obj_in   = cmd->obj_in;
        int obj_out  = cmd->obj_out;
        int obj_aux  = cmd->obj_aux;
        switch (cmd->type) {
            case CREATE:
            vec[obj_out] = rrb_create();
            break;
            case UNREF:
            rrb_unref(vec[obj_in]);
            break;
            case UPDATE:
            vec[obj_out] = rrb_update(vec[obj_in], cmd->index, &cmd->data.as_int);
            break;
            case PUSH:
            vec[obj_out] = rrb_push(vec[obj_in], &cmd->data.as_int);
            break;
            case POP:
            ;int* data;
            vec[obj_out] = rrb_pop(vec[obj_in], &data);
            break;
            case MERGE:
            vec[obj_out] = rrb_merge(vec[obj_in], vec[obj_aux]);
            break;
            case SPLIT:
            rrb_split(vec[obj_in], &vec[obj_out], &vec[obj_aux], cmd->index);
            break;
            case LOOKUP:
            rrb_lookup(vec[obj_in], cmd->index);
            break;
            case SIZE:
            rrb_size(vec[obj_in]); // hopefully won't be optimized out.
            break;
            case DUMP:
            rrb_ppp_leafs(vec[obj_in]);
            break;
            default: // mostly to remove warnings.
            fprintf(stderr, "Unsupported operation %d. Skipping.\n", cmd->type);
        }
    }

    gettimeofday(&t2, NULL);
    double elapsed_time = (t2.tv_sec - t1.tv_sec) * 1000.0;
    elapsed_time += (t2.tv_usec - t1.tv_usec) / 1000.0;
    return elapsed_time;
}


double execute_vector (Prog* prog) {
    rrb_t** vec = malloc(prog->nb_var * sizeof(*vec));

    eval_vector_cmds(prog->init, prog->init_size, vec);
    return eval_vector_cmds(prog->bench, prog->bench_size, vec);
}

int main (int argc, char* argv[]) {

  char* filename = NULL;

  struct option long_options[] = {
    { "file", required_argument, NULL, 'f' },
    { "test", no_argument, NULL, 't'},
    { "bench", no_argument, NULL, 'b'} };

  char c;
  int option_index = 0;
  while ((c = getopt_long(argc, argv, "f:bt", long_options, &option_index)) != -1) {
    switch (c) {
    case 'f':
      filename = optarg;
      break;
    case 't':
      is_test = 1;
      break;
    case 'b':
      is_bench = 1;
      break;
    default:
      fprintf(stderr, "Unknown option %c. Ignoring it.\n", c);
      exit (EXIT_FAILURE);
    }
  }
  if ( filename == NULL) {
    fprintf(stderr, "Filename missing. Aborting.\n");
    exit (EXIT_FAILURE);
  }

  Prog* prog = read_file(filename);

  if (! ( prog->implem & IMPLEM )) {
    fprintf(stderr, "Benchmark '%s' doesn't support %s. Aborting\n",
	    argv[1], IMPLEM_NAME);
    exit (EXIT_FAILURE);
  }

  if ( ! is_test && ! is_bench ) {
    fprintf(stderr, "Mode isn't specified. Assuming test.\n");
    is_test = 1;
  }

  double time = 0;
  if (is_test) {
    if (prog->struc == VECTOR) {
      time = execute_vector(prog);
    } else {
      // time = execute_map(prog);
    }
    printf("Time elapsed: %.3fms\n", time);
  }
  else if (is_bench) {
    for (int i = 0; i < 100; i++) {
      if (prog->struc == VECTOR) {
	    time += execute_vector(prog);
      } else {
	    //time += execute_map(prog);
      }
    }
    time /= 1000;
    printf("Total time: %.6fs\n", time);
    printf("Average time: %.6fs\n", time / 100);
  }

  return 0;

}
