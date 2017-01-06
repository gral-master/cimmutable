#include "rrb_dumper.h"

void tabulation(int number, FILE *file) {
    fprintf(file, "%*s", number, "");
}

void dump_nodes(rrb_vector_t *rrb, FILE *file, int tab) {
    tabulation(tab, file);
    fprintf(file, "node%p[label = \"", rrb);
    if (rrb->leafs == true) {
        for (int i = 0; i < 32; i++) {
            if (rrb->children.data[i] != NULL) {
                if (i == 0) fprintf(file, "<f%p> %d", rrb->children.data[i], i);
                else fprintf(file, " | <f%p> %d", rrb->children.data[i], i);
            }
        }
        fprintf(file, "\"];\n");
        return;
    }
    for (int i = 0; i < 32; i++) {
        if (rrb->children.arr[i] != NULL) {
            if (i == 0) fprintf(file, "<f%p> %d", rrb->children.arr[i], i);
            else fprintf(file, " | <f%p> %d", rrb->children.arr[i], i);
        }
    }
    fprintf(file, "\"];\n");
    for (int i = 0; i < 32; i++) {
        if (rrb->children.arr[i] != NULL) {
            dump_nodes(rrb->children.arr[i], file, tab);
        }
    }
}

void dump_edges(rrb_vector_t *rrb, FILE *file, int tab) {
    tabulation(tab, file);
    if (rrb->leafs == true) {
        return;
    }
    for (int i = 0; i < 32; i++) {
        if (rrb->children.arr[i] != NULL) {
            fprintf(file, "\"node%p\":f%p -> \"node%p\";\n", rrb, rrb->children.arr[i], rrb->children.arr[i]);
        }
    }
    for (int i = 0; i < 32; i++) {
        if (rrb->children.arr[i] != NULL) {
            dump_edges(rrb->children.arr[i], file, tab);
        }
    }
}

void rrb_dump(rrb_vector_t *rrb, char *file) {
    FILE *fh = fopen(file, "w");
    fprintf(fh, "digraph rrbtree {\n");
    tabulation(2, fh);
    fprintf(fh, "node [shape = record, height = 0.1];\n");
    dump_nodes(rrb, fh, 2);
    fprintf(fh, "}\n");
    fclose(fh);
}

void pp(const rrb_vector_t *rrb, int tab) {
    if (rrb != NULL) {
        if (rrb->leafs == true) {
            tabulation(tab, stdout);
            printf("Node {\n");
            for (int i = 0; i < 32; i++) {
                if (rrb->children.data[i] != NULL) {
                    tabulation(tab + 2, stdout);
                    printf("Leaf : %d\n", *rrb->children.data[i]);
                }
            }
            tabulation(tab, stdout);
            printf("}\n");
        } else {
            for (int i = 0; i < 32; i++) {
                if (rrb->children.arr[i] != NULL) {
                    tabulation(tab, stdout);
                    printf("Node {\n");
                    pp(rrb->children.arr[i], tab + 2);
                    tabulation(tab, stdout);
                    printf("}\n");
                }
            }
        }
    }
}

void rrb_pp(const rrb_vector_t *rrb) {
    pp(rrb, 0);
}

void ppp(const rrb_vector_t *rrb, int tab, bool leafs) {
    if (rrb != NULL) {
        if (rrb->leafs == true) {
            tabulation(tab, stdout);
            printf("Node %p {", rrb);
            tabulation(tab, stdout);
            printf("}\n");
            if (leafs == false) {
                return;
            }
            for (int i = 0; i < 32; i++) {
                if (rrb->children.data[i] != NULL) {
                    tabulation(tab + 2, stdout);
                    printf("%d: Leaf -> %d\n", i, *rrb->children.data[i]);
                }
            }
            tabulation(tab, stdout);
            printf("}\n");
        } else {
            tabulation(tab, stdout);
            printf("Node %p {\n", rrb);
            for (int i = 0; i < 32; i++) {
                if (rrb->children.arr[i] != NULL) {
                    ppp(rrb->children.arr[i], tab + 2, leafs);
                }
            }
            tabulation(tab, stdout);
            printf("}\n");
        }
    }
}

void rrb_ppp(const rrb_vector_t *rrb) {
    ppp(rrb, 0, false);
}

void rrb_ppp_leafs(const rrb_vector_t *rrb) {
    ppp(rrb, 0, true);
}
