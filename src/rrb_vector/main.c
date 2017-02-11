#include "rrb_vector.h"
#include "rrb_dumper.h"

#include "make_meta.c"

void newline() {
    printf("\n");
}

int main() {
    int* temp;
    int counter = 0, get = 564;

    rrb_t* rrb1 = rrb_create();

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

    rrb_t* meta = make_meta_vector();
    rrb_ppp_leafs(meta);

    temp = malloc(sizeof *temp);
    *temp = counter++;
    meta = rrb_push(meta, temp);
    rrb_ppp_leafs(meta);

    printf("\nGet number %d: %d\n", get, *rrb_lookup(meta, 62));

    imc_data_t* temp2;
    rrb_ppp_leafs(rrb_pop(meta, &temp2));

    rrb_t* left;
    rrb_t* right;
    rrb_split(rrb1, &left, &right, 50);

    printf("\nSplitted trees.\n\n");

    // rrb_ppp(rrb1);
    rrb_ppp(left);
    rrb_ppp(right);

    printf("\nSize of trees: %zu, %zu\n", rrb_size(left), rrb_size(right));

    return 0;
}
