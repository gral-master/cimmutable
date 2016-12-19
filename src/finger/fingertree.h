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

typedef struct view_t view;

struct view_t{
  void* elem;
  ft* fg;
};



ft* create_empty();
ft* create_single();
ft* create_deep();
node* create_data_node();
node* create_node_node();
/* preofsuff=0 for add on the left and 1 for an add on the right*/
ft* ft_add(void* data,ft* fgt,int preorsuf);
view ft_delete(ft*fgt,int preorsuf);
ft* ft_concat(ft* fin1,ft* fin2);
void ft_display(ft* fgt);
void node_display(node* node);
void checkInvariants();
ft* add_elem_deep_recur(ft* fgt,int preorsuff,node*data);
int check_available_space(ft* fgt,int preorsuf);
view ft_delete(ft* fgt,int preorsuf);
void copy_pref(ft*to,ft* from);
void copy_suff(ft*to,ft* from);
node ** get_right_infix(ft*res,int preorsuf, int inv);
#endif
