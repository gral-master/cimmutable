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
    int size;
} ft;

typedef struct {
    node* nodes[4];
    int size;
} affix;

typedef struct {
    affix* prefix; // node* prefix[4];
    affix* suffix; // node* suffix[4];
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


ft* create_empty();
ft* create_single(node* n);
ft* create_deep();
node* create_data_node(void* data);
node* create_node_node();
/* preofsuff=0 for add on the left and 1 for an add on the right*/
ft* ft_add(void* data,ft* fgt,int preorsuf);
view ft_delete(ft*fgt,int preorsuf);
ft* ft_concat(ft* fin1,ft* fin2);
void ft_display(ft* fgt);
void* ft_lookup(ft* ft, int index); //typedef imc_data_t;
void ft_unref(ft* ft);
void node_display(node* node);
ft* add_elem_deep_recur(ft* fgt,int preorsuff,node*data);
int check_available_space(ft* fgt,int preorsuf);
view ft_delete(ft* fgt,int preorsuf);
split ft_split(ft* fgt, int index);
void copy_pref(ft*to,ft* from);
void copy_suff(ft*to,ft* from);
affix* get_right_affix(ft*res,int preorsuf, int inv);
ft* concat_w_middle(ft* ft1, list* l,ft* ft2);
list* nodes(list*l);
list* affix_to_list(ft*fg,int preorsuf);
void invers_recursif(node*elem);
void add_elems_node(node* new_node,affix* old_affix,int preorsuf );
void update_affix(affix* new_affix,affix* old_affix,int preofsuf,node* data_node);
void shift_elements(affix* newaffix,int index,int preorsuf);
void remove_from_affix(affix* new_affix,affix* old_affix,int index,int preorsuf);
ft* rand_gene(int nbelem,int*elems);

#endif
