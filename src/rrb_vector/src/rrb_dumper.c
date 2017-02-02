#include "rrb_dumper.h"

/** Adds tabulation to the file, to pad text. */
void tabulation(int number, FILE* file) {
    fprintf(file, "%*s", number, "");
}

/** Dump the nodes into the dotfile. */
void dump_nodes(rrb_t* rrb, FILE* file, int tab) {
    tabulation(tab, file);
    fprintf(file, "node%p[label = \"", rrb);
    if (rrb->level == 1) {
        for (int i = 0; i < 32; i++) {
            if (rrb->nodes.leaf[i] != NULL) {
                if (i == 0) fprintf(file, "<f%p> %d", rrb->nodes.leaf[i], i);
                else fprintf(file, " | <f%p> %d", rrb->nodes.leaf[i], i);
            }
        }
        fprintf(file, "\"];\n");
        return;
    }
    for (int i = 0; i < 32; i++) {
        if (rrb->nodes.child[i] != NULL) {
            if (i == 0) fprintf(file, "<f%p> %d", rrb->nodes.child[i], i);
            else fprintf(file, " | <f%p> %d", rrb->nodes.child[i], i);
        }
    }
    fprintf(file, "\"];\n");
    for (int i = 0; i < 32; i++) {
        if (rrb->nodes.child[i] != NULL) {
            dump_nodes(rrb->nodes.child[i], file, tab);
        }
    }
}

/** Dump the edges into the dotfile. */
void dump_edges(rrb_t* rrb, FILE* file, int tab) {
    tabulation(tab, file);
    if (rrb->level == 1) {
        return;
    }
    for (int i = 0; i < 32; i++) {
        if (rrb->nodes.child[i] != NULL) {
            fprintf(file, "\"node%p\":f%p -> \"node%p\";\n", rrb, rrb->nodes.child[i], rrb->nodes.child[i]);
        }
    }
    for (int i = 0; i < 32; i++) {
        if (rrb->nodes.child[i] != NULL) {
            dump_edges(rrb->nodes.child[i], file, tab);
        }
    }
}

/** Dump the structure into a dotfile. */
void rrb_to_dotfile(rrb_t* rrb, char* path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        fprintf(stderr, "Unable to open %s\n", path);
        exit(EXIT_FAILURE);
    } else {
        fprintf(file, "digraph rrbtree {\n");
        tabulation(2, file);
        fprintf(file, "node [shape = record, height = 0.1];\n");
        dump_nodes(rrb, file, 2);
        fprintf(file, "}\n");
        fclose(file);
    }
}

/** Pretty print the structure into the file. */
void pp(const rrb_t* rrb, int tab, FILE* file) {
    if (rrb != NULL) {
        if (rrb->level == 1) {
            tabulation(tab, file);
            fprintf(file, "Node {\n");
            for (int i = 0; i < 32; i++) {
                if (rrb->nodes.leaf[i] != NULL) {
                    tabulation(tab + 2, file);
                    fprintf(file, "Leaf : %d\n", *rrb->nodes.leaf[i]);
                }
            }
            tabulation(tab, file);
            fprintf(file, "}\n");
        } else {
            for (int i = 0; i < 32; i++) {
                if (rrb->nodes.child[i] != NULL) {
                    tabulation(tab, file);
                    fprintf(file, "Node {\n");
                    pp(rrb->nodes.child[i], tab + 2, file);
                    tabulation(tab, file);
                    fprintf(file, "}\n");
                }
            }
        }
    }
}

/** Pretty print the structure into the stdout. */
void rrb_pp(const rrb_t* rrb) {
    pp(rrb, 0, stdout);
}

/** Pretty print the structure into the file indicated by path. */
void rrb_pp_file(const rrb_t* rrb, char* path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        fprintf(stderr, "Unable to open file %s\n", path);
        exit(EXIT_FAILURE);
    } else {
        pp(rrb, 0, file);
        fclose(file);
    }
}

/** Pretty print the structure with pointers to the file, w/ or w/o leafs. */
void ppp(const rrb_t* rrb, int tab, bool leafs, FILE* file) {
    if (rrb != NULL) {
        if (rrb->level == 1) {
            tabulation(tab, file);
            fprintf(file, "Node %p { ", rrb);
            if (leafs == false) {
                fprintf(file, "Size : %zu, ", rrb_size(rrb));
                fprintf(file, "Level : %d }\n", rrb->level);
                return;
            }
            fprintf(file, "\n");
            tabulation(tab + 2, file);
            fprintf(file, "Size : %zu\n", rrb_size(rrb));
            tabulation(tab + 2, file);
            fprintf(file, "Level : %d\n", rrb->level);
            if (rrb->meta != NULL) {
                tabulation(tab + 2, file);
                fprintf(file, "Meta : [");
                for (int i = 0; i < 32; i++) {
                    if (i < 31) {
                        fprintf(file, "%d, ", rrb->meta[i]);
                    } else {
                        fprintf(file, "%d]\n", rrb->meta[i]);
                    }
                }
            }
            for (int i = 0; i < 32; i++) {
                if (rrb->nodes.leaf[i] != NULL) {
                    tabulation(tab + 2, file);
                    fprintf(file, "%d: Leaf -> %d\n", i, *rrb->nodes.leaf[i]);
                }
            }
            tabulation(tab, file);
            fprintf(file, "}\n");
        } else {
            tabulation(tab, file);
            fprintf(file, "Node %p {\n", rrb);
            tabulation(tab + 2, file);
            fprintf(file, "Size : %zu\n", rrb_size(rrb));
            tabulation(tab + 2, file);
            fprintf(file, "Level : %d\n", rrb->level);
            if (rrb->meta != NULL) {
                tabulation(tab + 2, file);
                fprintf(file, "Meta : [");
                for (int i = 0; i < 32; i++) {
                    if (i < 31) {
                        fprintf(file, "%d, ", rrb->meta[i]);
                    } else {
                        fprintf(file, "%d]\n", rrb->meta[i]);
                    }
                }
            }
            for (int i = 0; i < 32; i++) {
                if (rrb->nodes.child[i] != NULL) {
                    ppp(rrb->nodes.child[i], tab + 2, leafs, file);
                }
            }
            tabulation(tab, stdout);
            fprintf(file, "}\n");
        }
    }
}

/** Pretty print the structure with pointers to the console. */
void rrb_ppp(const rrb_t* rrb) {
    ppp(rrb, 0, false, stdout);
}

/** Pretty print the structure with pointers and leafs to the console. */
void rrb_ppp_leafs(const rrb_t* rrb) {
    ppp(rrb, 0, true, stdout);
}

/** Pretty print the structure with pointers to the file indicated by path. */
void rrb_ppp_file(const rrb_t* rrb, char* path) {
    FILE* file = fopen(path, "w");
    if (file == NULL) {
        fprintf(stderr, "Unable to open file %s\n", path);
        exit(EXIT_FAILURE);
    } else {
        ppp(rrb, 0, false, file);
        fclose(file);
    }
}
/** Pretty print the structure with pointers and leafs to the file indicated by path. */
void rrb_ppp_leafs_file(const rrb_t* rrb, char* path) {
    FILE* file = fopen(path, "w");
    if (file == NULL) {
        fprintf(stderr, "Unable to open file %s\n", path);
        exit(EXIT_FAILURE);
    } else {
        ppp(rrb, 0, true, file);
        fclose(file);
    }
}
