#include "rrb_vector.h"

rrb_t *make_meta_vector() {
    int *temp;
    int counter = 0;

    rrb_t *rrb0 = rrb_create();
    for (int i = 0; i < 32; i++) {
        temp = malloc(sizeof *temp);
        *temp = counter++;
        if (i % 3 == 0) {
            rrb0 = rrb_push(rrb0, temp);
        } else if (i % 3 == 1) {
            rrb0 = rrb_push(rrb0, temp);
        } else {
            rrb0 = rrb_push(rrb0, temp);
        }
    }

    rrb_t *rrb1 = rrb_create();
    for (int i = 0; i < 10; i++) {
        temp = malloc(sizeof *temp);
        *temp = counter++;
        if (i % 3 == 0) {
            rrb1 = rrb_push(rrb1, temp);
        } else if (i % 3 == 1) {
            rrb1 = rrb_push(rrb1, temp);
        } else {
            rrb1 = rrb_push(rrb1, temp);
        }
    }

    rrb1->full = true;

    rrb_t *rrb2 = rrb_create();
    for (int i = 0; i < 20; i++) {
        temp = malloc(sizeof *temp);
        *temp = counter++;
        if (i % 3 == 0) {
            rrb2 = rrb_push(rrb2, temp);
        } else if (i % 3 == 1) {
            rrb2 = rrb_push(rrb2, temp);
        } else {
            rrb2 = rrb_push(rrb2, temp);
        }
    }

    rrb_t *rrb3 = malloc(sizeof *rrb3);
    rrb3->level = 2;
    rrb3->ref = 1;
    rrb3->elements = 62;
    rrb3->meta = calloc(32, sizeof *rrb3->meta);
    rrb3->meta[0] = 32;
    rrb3->meta[1] = 42;
    rrb3->meta[2] = 62;
    rrb3->full = false;
    rrb3->nodes.child = calloc(32, sizeof *rrb3->nodes.child);
    rrb3->nodes.child[0] = rrb0;
    rrb3->nodes.child[1] = rrb1;
    rrb3->nodes.child[2] = rrb2;

    rrb1->ref = 2;
    rrb2->ref = 2;

    return rrb3;
}
