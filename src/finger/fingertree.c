#include <stdio.h>
#include <stdlib.h>
#include "fingertree.h"

ft* create_empty() {
  ft* fgt = malloc(sizeof(ft));
  fgt->type = EMPTY_TYPE;
  fgt->ref_count = 1;
  fgt->size = 0;
  return fgt;
}

ft* create_single(){
  ft* fgt = malloc(sizeof(ft));
  fgt->type = SINGLE_TYPE;
  fgt->true_ft = malloc(sizeof(true_ft_t));
  fgt->ref_count = 1;
  fgt->size = 1;
  return fgt;
}


ft* create_deep(){
  ft* fgt = malloc(sizeof(ft));
  int i;
  fgt->type = DEEP_TYPE;
  fgt->true_ft = malloc(sizeof(true_ft_t));
  fgt->true_ft->d = malloc(sizeof(deep));
  for(i=0;i<4;i++){
    fgt->true_ft->d->prefix[i]= NULL;
    fgt->true_ft->d->suffix[i]= NULL;
  }

  fgt->true_ft->d->deeper = create_empty();
  fgt->ref_count = 1;

  fgt->size = 0; // The caller of this function should put the right size.

  return fgt;
}

node* create_data_node(){
  node* n = malloc(sizeof(node));
  n->type = DATA_TYPE;
  n->true_node = malloc(sizeof(true_node_t));
  n->ref_count = 1;
  n->size = 1;
  return n;
}

node* create_node_node(){
  int i;
  node* n = malloc(sizeof(node));
  n->type = NODE_TYPE;
  n->true_node = malloc(sizeof(true_node_t));
  for(i=0;i<2;i++)
    n->true_node->internal_node[i]=NULL;
  n->ref_count = 1;
  n->size=0;
  return n;
}


/*
 *this function adds an element to the right end of the finger 
 *For the moment, only works to add 1 element */
ft* ft_add(void* data, ft* fgt,int preorsuf) {
  ft* res;
  node* new_elem = create_data_node();
  new_elem->true_node->data=data;
  res=add_elem_deep_recur(fgt,preorsuf,new_elem);
  res->size = fgt->size+1;
  return res;
}

/*
 * For the moment, only works for trees with 1 element */
void node_display(node* node) {
  if (node == NULL)
    printf("NULL");
  switch (node->type) {
   
  case DATA_TYPE:
    printf("%d", *((int*)node->true_node->data));
    break;
  case NODE_TYPE:
    printf("Node(");
    for (int i = 0; i < 3; i++){
      node_display(node->true_node->internal_node[i]);
      printf(",");
    }
    printf(")");
    break;
  default:
    printf("_");
  }
}


/*
 * For the moment, only works for trees with 1 element */
void ft_display(ft* fgt) {
  if(fgt==NULL)
    {puts("NULL!!!");exit(-1);}
  switch (fgt->type) {
  case EMPTY_TYPE:
    printf("EMPTY");
    break;
  case SINGLE_TYPE:
    printf("Single(");
    node_display(fgt->true_ft->single);
    printf(")");
    break;
  case DEEP_TYPE:
    printf("Deep([");

    // Prefix
    for (int i = 0; i < 4; i++){
      if (fgt->true_ft->d->prefix[i] != NULL){
	node_display(fgt->true_ft->d->prefix[i]);
	printf(",");}
      }
    printf("],");
    // Deeper
    ft_display(fgt->true_ft->d->deeper);
    // Suffix
    printf(",[");
    for (int i = 3; i >=0; i--){
      if (fgt->true_ft->d->suffix[i] != NULL){
	node_display(fgt->true_ft->d->suffix[i]);
	printf(",");}
    }
	
    printf("])");
    break;
  default:
    printf("Error: Unsupported\n");
  }
}

/* 
 * this functions is recursive it buils the resulting finger tree.. the recurisvity occurs 
 * when there is no more space left in the prefix or suffix
 */
ft* add_elem_deep_recur(ft* fgt,int preorsuf,node*data){
  ft*res;
   node**t;
    node**r;
  if(fgt->type==EMPTY_TYPE){
    res = create_single();
    res->true_ft->single=data;
  }
  else if(fgt->type == SINGLE_TYPE) {
    res = create_deep();
    fgt->true_ft->single->ref_count++;
    if(preorsuf){
      t=res->true_ft->d->suffix;
      r=res->true_ft->d->prefix;
    }
    else{
      r=res->true_ft->d->suffix;
      t=res->true_ft->d->prefix;
    }
    r[0] = fgt->true_ft->single;
    t[0]=data;
  }
  else{
    res = create_deep();
    int index=check_available_space(fgt,preorsuf,0);
   
    if(preorsuf){
      t=fgt->true_ft->d->suffix;
      r=res->true_ft->d->suffix;
    }
    else{
      t=fgt->true_ft->d->prefix;
      r=res->true_ft->d->prefix;
    }
    int i;
    /* copy the pref and suff*/
 
    for(i=0;i<4;i++){
      res->true_ft->d->prefix[i]=fgt->true_ft->d->prefix[i];
      if(fgt->true_ft->d->prefix[i]!=NULL)fgt->true_ft->d->prefix[i]->ref_count++;
      res->true_ft->d->suffix[i]=fgt->true_ft->d->suffix[i];
      if(fgt->true_ft->d->suffix[i]!=NULL)fgt->true_ft->d->suffix[i]->ref_count++;
    }
 
    if(index==-1){
      node* new_node = create_node_node();
      int i;
      for(i=0;i<3;i++){
	new_node->true_node->internal_node[i]=create_data_node();
	new_node->true_node->internal_node[i]->true_node->data= fgt->true_ft->d->suffix[i+1]->true_node->data;
      }
      /* creating the new suffix or prefix*/
      r[0]=data;
      r[1]=t[0];r[2]=NULL;r[3]=NULL;
      /* recursive call to the same function with deeper*/
      res->true_ft->d->deeper=add_elem_deep_recur(fgt->true_ft->d->deeper,preorsuf,new_node);
     
    
    }
    else{
      /* we can add directly, there is space left in prefix or suffix*/
      res->true_ft->d->deeper = fgt->true_ft->d->deeper;
      fgt->true_ft->d->deeper->ref_count++;
      for(i=index-1;i>=0;i--)
	r[i+1]=r[i];

      /* add the new element*/
      t[0]->ref_count--;
      r[0]=data;
     }
  }
        
  return res;}

/* checks if there is available space in the suffix of the deep
 * to make it generic we use 
 * preorsuf to search in the prefix or suffix (0 for prefix, 1 for suffix)
 * deepness to konw at which level of the tree we are
 */
int check_available_space(ft* fgt,int preorsuf, int deepness){
  if(fgt->type!=DEEP_TYPE){
    printf("error not a deep\n");
    exit(-1);}
  node**t;
  if(preorsuf){
    t = fgt->true_ft->d->suffix;}
  else{
    t = fgt->true_ft->d->prefix;}
  
  int i,res=-1;
  for(i=0;i<4;i++){
    if(t[i]==NULL){
      res=i;
      break;
    }
  }
  return res;
}
 
void checkInvariants() {
  // === Node Invariants===
  // Nodes are either NODE_TYPE or DATA_TYPE

  // A DATA_TYPE Node does not contain another Node

  // A NODE_TYPE Node does contain 2 or 3 Nodes

  // ===Finger tree Invariant===
  // Finger trees are either DEEP_TYPE, SINGLE_TYPE or EMPTY_TYPE

  // A SINGLE_TYPE Finger tree contains only one Node (and no infix).

  // Any Node in the infix and any Single at level 0 (the highest level) are
  // DATA_TYPE.

  // Any Node in the infix and any Single at level n (the deeper level, the
  // greater n) are NODE_TYPE^n(DATA_TYPE).

  // A DEEP_TYPE Finger tree contains infixes and a deeper Finger tree.

  // The infixes contain at least one elem each (i.e. at least 1 Node contains
  // effective data)

  // The infixes are arrays of size four (i.e. contains 4 Node, even if
  // unused)

  // In an infix containing n elements, these elements correspond to the n
  // leftmost indexes of the array.

  // ===ref_count Invariants===

  // ===size Invariants===

}
