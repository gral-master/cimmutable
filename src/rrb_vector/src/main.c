#include "rrb_vector.h"
#include "rrb_dumper.h"

void newline() {
    printf("\n");
}

int main() {
    int x = 2, y = 3, z = 4;

    rrb_vector_t *rrb1 = rrb_create();

    for (int i = 0; i < 1024; i++) {
        if (i % 3 == 0) {
            rrb1 = rrb_add(rrb1, &x);
        } else if (i % 3 == 1) {
            rrb1 = rrb_add(rrb1, &y);
        } else {
            rrb1 = rrb_add(rrb1, &z);
        }
    }
    rrb_ppp(rrb1);

    return 0;
}
