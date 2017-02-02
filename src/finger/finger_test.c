#include <stdio.h>
#include <stdlib.h>
#include "fingers.h"

void display(finger_data_t** data, int size) {
    for (int i = 0; i < size - 1; i++) {
        fprintf(stdout, "%d, ", *data[i]);
    }
    fprintf(stdout, "%d\n", *data[size - 1]);
}

int main(void) {
    deep_t* tree = make_empty_node();

    int size = 1000;

    fprintf(stdout, "Append\n");
    for (int i = 0; i < size; i++) {
        fprintf(stderr, "%d, ", i);
        int* data = malloc(sizeof(int));
        *data = i;
        tree = append(tree, data, FINGER_RIGHT);
        if (!tree)
            fprintf(stderr, "what the shit\n");
    }
    fprintf(stdout, "Size: %d (should be %d)\n", vector_size(tree), size);

    fprintf(stdout, "Lookup\n");
    fprintf(stdout, "%d\n", *lookup(tree, 2));

    dump_deep_debug(tree, 0, display);

    fprintf(stdout, "\nUpdate\n");
    int* upd = malloc(sizeof(int));
    *upd = 589420;
    tree = update_deep(tree, 0, upd);

    dump_deep_debug(tree, 0, display);

    fprintf(stdout, "pop \n");
    int* pop_val;
    tree = pop(tree, &pop_val);
    fprintf(stdout, "Last value: %d\n", *pop_val);
    fprintf(stdout, "Remaining values:\n");
    dump_deep(tree, 0, display);
}
