#include "rrb_vector.h"
#include "rrb_dumper.h"

rrb_vector_t *make_meta_vector() {
    int *temp;
    int counter = 0;

    rrb_vector_t *rrb0 = rrb_create();
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


    rrb_vector_t *rrb1 = rrb_create();
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

    rrb_vector_t *rrb2 = rrb_create();
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

    rrb_vector_t *rrb3 = malloc(sizeof *rrb3);
    rrb3->level = 2;
    rrb3->ref = 1;
    rrb3->elements = 62;
    rrb3->meta = calloc(32, sizeof *rrb3->meta);
    rrb3->meta[0] = 32;
    rrb3->meta[1] = 42;
    rrb3->meta[2] = 62;
    rrb3->full = false;
    rrb3->leafs = false;
    rrb3->children.arr = calloc(32, sizeof *rrb3->children.arr);
    rrb3->children.arr[0] = rrb0;
    rrb3->children.arr[1] = rrb1;
    rrb3->children.arr[2] = rrb2;

    rrb1->ref = 2;
    rrb2->ref = 2;

    return rrb3;
}

void newline() {
    printf("\n");
}

int main() {
    int *temp;
    int counter = 0, get = 564;

    rrb_vector_t *rrb1 = rrb_create();

    for (int i = 0; i < 1025; i++) {
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
    rrb_ppp(rrb1);

    printf("\nGet number %d: %d\n", get, *rrb_lookup(rrb1, get));

    printf("Get size of rrb1: %zu\n\n", rrb_size(rrb1));

    temp = malloc(sizeof *temp);
    *temp = counter++;
    rrb1 = rrb_update(rrb1, 23, temp);

    rrb_ppp(rrb1);

    rrb_vector_t *meta = make_meta_vector();
    rrb_ppp_leafs(meta);

    temp = malloc(sizeof *temp);
    *temp = counter++;
    meta = rrb_push(meta, temp);
    rrb_ppp_leafs(meta);

    printf("\nGet number %d: %d\n", get, *rrb_lookup(meta, 62));

    imc_data_t *temp2;
    rrb_ppp_leafs(rrb_pop(meta, &temp2));

    return 0;
}
