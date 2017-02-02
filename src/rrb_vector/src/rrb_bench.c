#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "rrb_vector.h"
#include "rrb_dumper.h"
#include "parser.h"

// IMPLEM : AVL or RRB or FINGER
#define IMPLEM AVL
// IMPLEM_NAME : doesn't matter, only here for the prints.
#define IMPLEM_NAME "avl"

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

    if (!(prog->data_type == INT)) {
        fprintf(stderr, "Not int.\n");
        exit(EXIT_SUCCESS);
    }

    double time = 0;

    // for (int i = 0; i < 1000; i++) {
        if (prog->struc == VECTOR) {
            time += execute_vector(prog);
        } else {
            fprintf(stderr, "Not map.\n");
            exit(EXIT_SUCCESS);
        }
    // }

    printf("%.3f\n", time/1);

    return 0;
}
