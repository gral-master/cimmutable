#ifndef FINGER_H
#define FINGER_H

//#define CHECK_SAFETY

// Nodes
typedef imc_data_t;
//typedef imc_key_t;

enum node_type {NODE_TYPE, DATA_TYPE};

typedef union true_node_t true_node_t;

typedef struct {
    true_node_t* true_node;
    enum node_type type;
    //imc_key_t key;
    int size;
    int ref_count;
} node;

union true_node_t {
    imc_data_t* data;
    node* internal_node[3];
};


typedef struct list_t {
    node* elem;
    struct list_t * next;
} list;


// Finger Tree (Single, Deep)
enum ft_type {DEEP_TYPE, SINGLE_TYPE, EMPTY_TYPE};

typedef union true_ft_t true_ft_t;

typedef struct {
    true_ft_t* true_ft;
    enum ft_type type;
    int ref_count;
    //imc_key_t key;
    int size;
} ft;

typedef struct {
    node* nodes[4];
    //imc_key_t key;
    int size;
} affix;

typedef struct {
    affix* prefix;
    affix* suffix;
    ft* deeper;
} deep;

union true_ft_t {
    node* single;
    deep* d;
};

typedef struct view_t view;

struct view_t{
  node* elem;
  ft* fg;
};

typedef struct {
    ft* ft1;
    node* node;
    ft* ft2;
} split;

typedef struct {
    affix* left;
    node* node;
    affix* right;
} splitnode;

node* create_data_node(void* data);
ft* create_empty();
ft* ft_add(imc_data_t* data,ft* fgt,int preorsuf);
view ft_delete(ft*fgt,int preorsuf);
ft* ft_concat(ft* fin1,ft* fin2);
void ft_display(ft* fgt);
imc_data_t* ft_lookup(ft* ft, int index);
void ft_unref(ft* ft);
void node_unref(node* n);
void node_display(node* node);
view ft_delete(ft* fgt,int preorsuf);
split ft_split(ft* fgt, int index);
ft* concat_w_middle(ft* ft1, list* l,ft* ft2);
ft* ft_generator(int nbelem,int*elems);
void simulation_add(int density_each_point, int shfit, int starting_size, int finishing_point);

#endif
