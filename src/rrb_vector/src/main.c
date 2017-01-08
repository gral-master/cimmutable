#include "rrb_vector.h"
#include "rrb_dumper.h"

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
            rrb1 = rrb_add(rrb1, temp);
        } else if (i % 3 == 1) {
            rrb1 = rrb_add(rrb1, temp);
        } else {
            rrb1 = rrb_add(rrb1, temp);
        }
    }
    rrb_ppp(rrb1);

    printf("Get number %d: %d\n", get, *rrb_lookup(rrb1, get));

    printf("Get size of rrb1: %zu\n", rrb_size(rrb1));

    return 0;
}
