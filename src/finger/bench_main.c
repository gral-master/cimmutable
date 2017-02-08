#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <getopt.h>

#include "imc_ft_map.h"
#include "imc_ft_vector.h"
#include "parser.h"

// IMPLEM : AVL or RRB or FINGER
#define IMPLEM FINGER
// IMPLEM_NAME : doesn't matter, only here for the prints.
#define IMPLEM_NAME "finger"

int is_test = 0, is_bench = 0;

double eval_vector_cmds(Prog* prog, command** cmds,
			int size, imc_ft_vector_t** vec) {
  struct timeval t1, t2;
  gettimeofday(&t1, NULL);
  
  for (int i = 0; i < size; i++) {
    command* cmd = cmds[i];
    int obj_in   = cmd->obj_in;
    int obj_out  = cmd->obj_out;
    int obj_aux  = cmd->obj_aux;
    switch (cmd->type) {
    case CREATE:
      vec[obj_out] = imc_ft_vector_create();
      break;
    case UNREF:
      imc_ft_vector_unref(vec[obj_in]);
      break;
    case UPDATE:
      vec[obj_out] = imc_ft_vector_update(vec[obj_in],
                                          cmd->index,
                                          &cmd->data.as_int);
      break;
    case PUSH:
            vec[obj_out] = imc_ft_vector_push(vec[obj_in], &cmd->data.as_int);
      break;
    case POP:
      ;void* data;
            vec[obj_out] = imc_ft_vector_pop(vec[obj_in], &data);
      break;
    case MERGE:
            vec[obj_out] = imc_ft_vector_merge(vec[obj_in], vec[obj_aux]);
      break;
    case SPLIT:
            imc_ft_vector_split(vec[obj_in], cmd->index, &vec[obj_out], &vec[obj_aux]);
      break;
    case LOOKUP:
            imc_ft_vector_lookup(vec[obj_in], cmd->index);
      break;
    case SIZE:
            imc_ft_vector_size(vec[obj_in]);
      break;
    case DUMP:
        if (is_test)
            imc_ft_vector_dump(vec[obj_in]);
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
  imc_ft_vector_t** vec = malloc(prog->nb_var * sizeof(*vec));

  eval_vector_cmds(prog, prog->init, prog->init_size, vec);
  return eval_vector_cmds(prog, prog->bench, prog->bench_size, vec);
}

/*
double eval_map_cmds(Prog* prog, command** cmds,
		     int size, avl_map_t** map) {
  struct timeval t1, t2;
  gettimeofday(&t1, NULL);
  
  for (int i = 0; i < size; i++) {
    command* cmd = cmds[i];
    int obj_in   = cmd->obj_in;
    int obj_out  = cmd->obj_out;
    switch (cmd->type) {
    case CREATE:
      map[obj_out] =
	avl_map_create(
	 prog->key_type  == INT ? int_box_as_string : string_box_as_string,
	 prog->data_type == INT ? int_box_as_string : string_box_as_string,
	 prog->key_type  == INT ? compare_int_keys  : compare_string_keys  );
      break;
    case UNREF:
      avl_map_unref(map[obj_in]);
      break;
    case UPDATE:
      map[obj_out] =
	avl_map_update(
	  map[obj_in],
	  prog->key_type  == INT ? make_int_box(cmd->key.as_int) :
	                           make_string_box(cmd->key.as_string),
	  prog->data_type == INT ? make_int_box(cmd->data.as_int) :
	                           make_string_box(cmd->data.as_string) );
      break;
    case REMOVE:
      ;void* data;
      map[obj_out] =
	avl_map_remove(map[obj_in], prog->key_type == INT ?
		       make_int_box(cmd->key.as_int) :
		       make_string_box(cmd->key.as_string), &data );
      break;
    case LOOKUP:
      avl_map_lookup(map[obj_in], prog->key_type == INT ?
		     make_int_box(cmd->key.as_int) :
		     make_string_box(cmd->key.as_string) );
      break;
    case SIZE:
      avl_map_size(map[obj_in]);
      break;
    case DUMP:
      avl_map_dump(map[obj_in]);
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


double execute_map (Prog* prog) {
  avl_map_t** map = malloc(prog->nb_var * sizeof(*map));

  eval_map_cmds(prog, prog->init, prog->init_size, map);
  return eval_map_cmds(prog, prog->bench, prog->bench_size, map);
}
*/

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
      //time = execute_map(prog);
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