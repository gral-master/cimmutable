#ifndef FINGER_H
#define FINGER_H

// Nodes
enum node_type {NODE_TYPE, DATA_TYPE};

typedef union true_node_t true_node_t;

typedef struct {
    true_node_t* true_node;
    enum node_type type;
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
  
ft* ft_init();
void ft_add(void* data,ft* fgt);
ft* ft_concat(ft* fin1,ft* fin2);
void ft_display(ft* fgt);
void node_display(node* node);
void create_single(ft* fgt);
void create_deep(ft* fgt);
void checkInvariants();
#endif
