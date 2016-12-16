#ifndef FINGER_H
#define FINGER_H

// Nodes
enum node_type {NODE_TYPE, DATA_TYPE};

typedef union true_node_t true_node_t;

typedef struct {
    true_node_t* true_node;
    enum node_type type;
    int size;
    int ref_count;
} node;

union true_node_t {
    void* data;  //typedef imc_data_t;
    node* internal_node[3];
};

// Finger Tree (Single, Deep)
enum ft_type {DEEP_TYPE, SINGLE_TYPE, EMPTY_TYPE};

typedef union true_ft_t true_ft_t;

typedef struct {
    true_ft_t* true_ft;
    enum ft_type type;
    int ref_count;
    int size;
} ft;

typedef struct {
    node* prefix[4];
    node* suffix[4];
    ft* deeper;
} deep;

union true_ft_t {
    node* single;
    deep* d;
};

ft* create_empty();
ft* create_single();
ft* create_deep();
node* create_data_node();
ft* ft_add(void* data,ft* fgt);
ft* ft_concat(ft* fin1,ft* fin2);
void ft_display(ft* fgt);
void node_display(node* node);
void checkInvariants();
ft* add_elem_deep_simple(ft* fgt,ft* old,int index,void* data);
ft* add_elem_deep_recur(ft* fgt,ft* old,void* data);
int check_available_space(ft* fgt);
#endif
