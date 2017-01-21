#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "fingertree.h"
#include "invariants.h"
#include "list.h"
#include "reflist.h"

ft* create_empty() {
    ft* fgt = malloc(sizeof(ft));
    fgt->type = EMPTY_TYPE;
    fgt->ref_count = 1;
    fgt->size = 0;
    return fgt;
}

ft* create_single(node* n) {
    ft* fgt = malloc(sizeof(ft));
    fgt->type = SINGLE_TYPE;
    fgt->true_ft = malloc(sizeof(true_ft_t));
    fgt->true_ft->single = n;
    fgt->ref_count = 1;
    fgt->size = n->size;
    return fgt;
}

ft* create_deep() {
    ft* fgt = malloc(sizeof(ft));
    int i;
    fgt->type = DEEP_TYPE;
    fgt->true_ft = malloc(sizeof(true_ft_t));
    fgt->true_ft->d = malloc(sizeof(deep));
    fgt->true_ft->d->prefix = malloc(sizeof(affix));
    fgt->true_ft->d->suffix = malloc(sizeof(affix));

    for(i=0;i<4;i++){
        fgt->true_ft->d->prefix->nodes[i] = NULL;
        fgt->true_ft->d->suffix->nodes[i] = NULL;
    }
    fgt->true_ft->d->prefix->size = 0;
    fgt->true_ft->d->suffix->size = 0;

    fgt->true_ft->d->deeper = create_empty();
    fgt->ref_count = 1;
    fgt->size = 0; // The caller of this function should put the right size.

    return fgt;
}

ft* create_deep_withoutdeeper() {
    ft* fgt = malloc(sizeof(ft));
    int i;
    fgt->type = DEEP_TYPE;
    fgt->true_ft = malloc(sizeof(true_ft_t));
    fgt->true_ft->d = malloc(sizeof(deep));
    fgt->true_ft->d->prefix = malloc(sizeof(affix));
    fgt->true_ft->d->suffix = malloc(sizeof(affix));
    
    for(i=0;i<4;i++){
        fgt->true_ft->d->prefix->nodes[i] = NULL;
        fgt->true_ft->d->suffix->nodes[i] = NULL;
    }
    fgt->true_ft->d->prefix->size = 0;
    fgt->true_ft->d->suffix->size = 0;
    
    fgt->ref_count = 1;
    fgt->size = 0; // The caller of this function should put the right size.
    
    return fgt;
}

node* create_data_node(void* data) {
    node* n = malloc(sizeof(node));
    n->type = DATA_TYPE;
    n->true_node = malloc(sizeof(true_node_t));
    n->true_node->data = data;
    n->ref_count = 1;
    n->size = 1;
    return n;
}

node* create_node_node() {
    int i;
    node* n = malloc(sizeof(node));
    n->type = NODE_TYPE;
    n->true_node = malloc(sizeof(true_node_t));

    for(i=0;i<2;i++) {
        n->true_node->internal_node[i] = NULL;
    }

    n->ref_count = 1;
    n->size = 0;
    return n;
}


void shift_elements(affix* new_affix,int index,int preorsuf){
  int i;
  if(preorsuf){
    for(i=index;i<3;i++) {
      new_affix->nodes[i]=new_affix->nodes[i+1];
    }

  }
  else{
    for(i=index-1;i>=0;i--) {
      new_affix->nodes[i+1]=new_affix->nodes[i];
    }
  }

}


void update_affix(affix* new_affix,affix* old_affix,int preorsuf,node* data_node){
  if(preorsuf){
    new_affix->nodes[3]=data_node;
    new_affix->nodes[2]=old_affix->nodes[3];
    new_affix->nodes[1]=NULL;
    new_affix->nodes[0]=NULL;
    new_affix->size=data_node->size + old_affix->nodes[3]->size;
  }
  else{
    new_affix->nodes[0]=data_node;
    new_affix->nodes[1]=old_affix->nodes[0];
    new_affix->nodes[2]=NULL;
    new_affix->nodes[3]=NULL;
    new_affix->size=data_node->size + old_affix->nodes[0]->size;
  }

}

void add_elems_node(node* new_node,affix* old_affix,int preorsuf ){
  int i;
  if(preorsuf){
    for(i=2;i>=0;i--){
      new_node->true_node->internal_node[i] = old_affix->nodes[i];
      new_node->size += old_affix->nodes[i]->size;
    }
  }
  else{
	     
    for(i=0;i<3;i++){
      new_node->true_node->internal_node[i] = old_affix->nodes[i+1];
      new_node->size += old_affix->nodes[i+1]->size;
    }
  }


}

list* nodes(list*l){
    node *tmp, *tmp2;
    list* res;

    if(l==NULL){
        printf("nodes: error list NULL\n");
        exit(-1);
    }
    else if(l->next==NULL){
        printf("nodes: error list->next =  NULL\n");
        exit(-1);
    }
    else if(l->next->next==NULL){
        // List of 2 -> Node2
        tmp = create_node_node();
        tmp->true_node->internal_node[0]= l->elem;
        tmp->true_node->internal_node[1]= l->next->elem;
        tmp->true_node->internal_node[2]= NULL;
	tmp->size = l->elem->size + l->next->elem->size;

        l=removel(l);
        l=removel(l);
        res = add(tmp,l);

    }
    else if(l->next->next->next==NULL){
        // List of 3 -> Node3
        tmp = create_node_node();
        tmp->true_node->internal_node[0]= l->elem;
        tmp->true_node->internal_node[1]= l->next->elem;
        tmp->true_node->internal_node[2]= l->next->next->elem;
	tmp->size = l->elem->size + l->next->elem->size+ l->next->next->elem->size;
        l=removel(l);
        l=removel(l);
        l=removel(l);
        res = add(tmp,l);
    }
    else if (l->next->next->next->next == NULL) {
        // List of 4 -> 2 Node2
        tmp = create_node_node();
        tmp->true_node->internal_node[0] = l->elem;
        tmp->true_node->internal_node[1] = l->next->elem;
       tmp->size = l->elem->size + l->next->elem->size;
        tmp2 = create_node_node();
        tmp2->true_node->internal_node[0] = l->next->next->elem;
        tmp2->true_node->internal_node[1] = l->next->next->next->elem;
	tmp2->size = l->next->next->elem->size + l->next->next->next->elem->size;
        l=removel(l);
        l=removel(l);
        l=removel(l);
        l=removel(l);

        res=add(tmp2,l);
        res=add(tmp,res);
    } else {
        // Default -> Node3, recursive call
        res = create_lempty();
        tmp = create_node_node();
        tmp->true_node->internal_node[0]= l->elem;
        tmp->true_node->internal_node[1]= l->next->elem;
        tmp->true_node->internal_node[2]= l->next->next->elem;
	tmp->size = l->elem->size + l->next->elem->size+ l->next->next->elem->size;
        l=removel(l);
        l=removel(l);
        l=removel(l);
        res = nodes(l);
        res = add(tmp,res);
    }
    return res;
}



void invers_recursif(node*elem){
  if(elem->type==DATA_TYPE)
    return;
  else{
    node**simpl=elem->true_node->internal_node;
    node*tmp=simpl[0];
    simpl[0]=simpl[2];
    simpl[2]=tmp;
    invers_recursif(simpl[0]);
    invers_recursif(simpl[1]);
    invers_recursif(simpl[2]);

  }

}

/* checks if there is available space in the suffix of the deep
* to make it generic we use
* preorsuf to search in the prefix or suffix (0 for prefix, 1 for suffix)
* Returns the next available index of the affix, or -1 if there is none.
*/
int check_available_space(ft* fgt,int preorsuf){


    if(fgt->type!=DEEP_TYPE){
        printf("error not a deep\n");
        exit(-1);
    }
    int i;
    affix*old_affix;
    if(preorsuf){
      old_affix = fgt->true_ft->d->suffix;
      int  j = old_affix->size;

      for (i = 3; i >= 0; i--) {
        if (j == 0)
            break;
        j -= old_affix->nodes[i]->size;
      }

      if (i < 0) {
        return -1;
      }

    }
    else{
         old_affix = fgt->true_ft->d->prefix;
      int j = old_affix->size;

      for (i = 0; i < 4; i++) {
        if (j == 0)
	  break;
        j -= old_affix->nodes[i]->size;
      }

      if (i > 3) {
        return -1;
      }

    }


    return i;
}


void copy_pref(ft*res,ft* fgt){
    int j;
    /* i did not see how to keep your code when changing convention*/
    /* for(i=0,j=0;i<fgt->true_ft->d->prefix->size;i += fgt->true_ft->d->prefix->nodes[j]->size,j++){ */
    for(j=0;j<4;j++){
      if(fgt->true_ft->d->prefix->nodes[j]==NULL)
	res->true_ft->d->prefix->nodes[j]=NULL;
      else{
        res->true_ft->d->prefix->nodes[j]=fgt->true_ft->d->prefix->nodes[j];
        fgt->true_ft->d->prefix->nodes[j]->ref_count++;
      }
    }
    res->true_ft->d->prefix->size = fgt->true_ft->d->prefix->size;
}

void copy_suff(ft*res,ft* fgt){
    int j;
    /* for(i=0,j=0;i<fgt->true_ft->d->suffix->size;i += fgt->true_ft->d->suffix->nodes[j]->size,j++){ */
    for(j=0;j<4;j++){
      if(fgt->true_ft->d->suffix->nodes[j]==NULL)
	res->true_ft->d->suffix->nodes[j]=NULL;
      else{
        res->true_ft->d->suffix->nodes[j]=fgt->true_ft->d->suffix->nodes[j];
        fgt->true_ft->d->suffix->nodes[j]->ref_count++;
      }
    }
    res->true_ft->d->suffix->size = fgt->true_ft->d->suffix->size;
}

affix* get_right_affix(ft*res,int preorsuf, int inv){
    affix*new_affix;
    if(inv){
        if(preorsuf)
        preorsuf=0;
        else
        preorsuf=1;
    }

    if(preorsuf) {
        new_affix=res->true_ft->d->suffix;
    } else {
        new_affix=res->true_ft->d->prefix;
    }
    return new_affix;
}

ft* ft_concat(ft* ft1,ft* ft2){
    // Preconditions & Invariants
    
    checkInvariants(ft1);
    checkInvariants(ft2);
     ft* res;
    list* l=NULL;
    res = concat_w_middle(ft1,l,ft2);
    
    // Postconditions & Invariants
    checkInvariants(res);
    return res;
}

void remove_from_affix(affix* new_affix,affix* old_affix,int index,int preorsuf){
  int i;
   if(preorsuf){
    new_affix->size = old_affix->size - new_affix->nodes[3]->size;
    for(i=3;i>index;i--) {
          new_affix->nodes[i]=new_affix->nodes[i-1];
    }
    
    new_affix->nodes[index]=NULL;
	
  }
  else{
    new_affix->size = old_affix->size - new_affix->nodes[0]->size;
    for(i=0;i<index;i++) {
      new_affix->nodes[i]=new_affix->nodes[i+1];
    }
    new_affix->nodes[index]=NULL;
	    

  }

}


/*
* this function is recursive. it builds the resulting finger tree
* the recursivity occurs when there is no space left in the prefix or suffix
*/
ft* add_elem_deep_recur(ft* fgt,int preorsuf,node*data_node){
    ft*res;
    affix*old_affix;
    affix*new_affix;
      // Save the reference counter of nodes and deeper of fgt.
    //    reflist* rl_fgt = rl_of_ft(fgt);
    //refdeep rd_fgt, rd_res;
    /* if (fgt->type == DEEP_TYPE && fgt->true_ft->d->deeper != NULL) { */
    /*     rd_fgt.elem = fgt->true_ft->d->deeper; */
    /*     rd_fgt.ref = rd_fgt.elem->ref_count; */
    /* } */
    /* else { */
    /*     rd_fgt.elem = NULL; */
    /*     rd_fgt.ref = 0; */
    /* } */
    
    if(fgt->type==EMPTY_TYPE){
        res = create_single(data_node);
    }
    else if(fgt->type == SINGLE_TYPE) {
      res = create_deep();
      if(preorsuf){
	old_affix=res->true_ft->d->suffix;
	new_affix=res->true_ft->d->prefix;
	new_affix->nodes[0] = fgt->true_ft->single;	
	old_affix->nodes[3] = data_node;
      }
      else{
	new_affix=res->true_ft->d->suffix;
	old_affix=res->true_ft->d->prefix;
        new_affix->nodes[3] = fgt->true_ft->single;	
        old_affix->nodes[0] = data_node;
      }
      fgt->true_ft->single->ref_count++;
      new_affix->size = fgt->true_ft->single->size;
      old_affix->size = data_node->size;
      res->size = new_affix->size + old_affix->size;
    }
    else{
        res = create_deep_withoutdeeper();
        int index=check_available_space(fgt,preorsuf);
        /* copy the pref and suff*/
        copy_pref(res,fgt);
        copy_suff(res,fgt);
        new_affix=get_right_affix(res,preorsuf,0);
        old_affix=get_right_affix(fgt,preorsuf,0);

        if(index==-1) {
            // There is not enough space in the affix
            node* new_node = create_node_node();
	    add_elems_node(new_node,old_affix,preorsuf );
            /* creating the new suffix or prefix*/
            update_affix(new_affix,old_affix,preorsuf,data_node);
            /* recursive call to the same function with deeper*/
            res->true_ft->d->deeper=add_elem_deep_recur(fgt->true_ft->d->deeper,
                preorsuf,new_node);
            res->size = res->true_ft->d->deeper->size + res->true_ft->d->prefix->size+res->true_ft->d->suffix->size;
        }
        else {
            /* we can add directly, there is space left in prefix or suffix*/
            res->true_ft->d->deeper = fgt->true_ft->d->deeper;
            fgt->true_ft->d->deeper->ref_count++;
	    shift_elements(new_affix,index,preorsuf);
      	    /* add the new element*/
	    if(preorsuf)
	      new_affix->nodes[3]=data_node;
	    else
	      new_affix->nodes[0]=data_node;
            new_affix->size += data_node->size;
            res->size = res->true_ft->d->deeper->size + res->true_ft->d->prefix->size+res->true_ft->d->suffix->size;
        }
    }
    
    // Compare the reference counters of nodes and deeper of fgt (before the function) and res
    /* reflist* rl_res = rl_of_ft(res); */
    /* inter_verify(rl_fgt, rl_res); */
    /* if (res->type == DEEP_TYPE && res->true_ft->d->deeper != NULL) { */
    /*     rd_res.elem = res->true_ft->d->deeper; */
    /*     rd_res.ref = rd_res.elem->ref_count; */
        
    /*     // Compare rd_res and rd_fgt */
    /*     if (rd_fgt.elem != NULL) { */
    /*         if (rd_fgt.elem == rd_res.elem) { */
    /*             assert(rd_fgt.ref + 1 == rd_res.ref); */
    /*         } */
    /*     } */
    /* } */
    /* free_reflist(rl_fgt); */
    /* free_reflist(rl_res); */

    return res;
}


/**
* Makes a list from the affix of a finger tree.
*/
list* affix_to_list(ft* fg,int preorsuf){
    affix* new_affix;
    list*l=NULL;
    if(preorsuf) {
        new_affix=fg->true_ft->d->suffix;
    } else {
        new_affix=fg->true_ft->d->prefix;
    }
    int i;
    for(i=0;i<4;i++){
        l=add(new_affix->nodes[i],l);
    }
    return l;
}


ft* ft_add(void* data, ft* fgt,int preorsuf) {
    // Preconditions & Invariants
    checkInvariants(fgt);

    ft* res;
    node* new_elem = create_data_node(data);
    res=add_elem_deep_recur(fgt,preorsuf,new_elem);
    res->size = fgt->size+1;

    // Postconditions & Invariants
    checkInvariants(fgt);
    return res;
}

void node_display(node* node) {
    if (node == NULL){
        printf("NULL");
        return;
    }
    switch (node->type) {
        case DATA_TYPE:
	  printf("(%d,%d)", *((int*)node->true_node->data),node->ref_count);
            break;
        case NODE_TYPE:
            printf("Node(");
            for (int i = 0; i < 3; i++){
                if (node->true_node->internal_node[i] != NULL) {
                    node_display(node->true_node->internal_node[i]);
                    printf(",");
                }
            }
            printf("rc=%d)", node->ref_count);
            break;
        default:
            printf("_");
    }
}

void ft_display(ft* fgt) {
  printf("ft ref_count : %d\n",fgt->ref_count);
    if(fgt==NULL)
    {
        puts("NULL!!!");
        exit(-1);
    }
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
                if (fgt->true_ft->d->prefix->nodes[i] != NULL){
		  printf("[%d]:",i);
                    node_display(fgt->true_ft->d->prefix->nodes[i]);
                    printf(",");
                }
            }
            printf("],");
            // Deeper
            ft_display(fgt->true_ft->d->deeper);
            // Suffix
            printf(",[");
            for (int i =0; i <4; i++){
                if (fgt->true_ft->d->suffix->nodes[i] != NULL){
		  printf("[%d]:",i);
                    node_display(fgt->true_ft->d->suffix->nodes[i]);
                    printf(",");
                }
            }
            printf("])");
            break;
        default:
            printf("Error: Unsupported\n");
    }
}




void* node_lookup(node* n, int index) {
    if (n->type == DATA_TYPE)
        return n->true_node->data;
    else {
        node** children = n->true_node->internal_node;
        
        for (int i = 0; i < 3; i++) {
            if (children[i] == NULL)
                continue;
            
            if (index < children[i]->size) {
                return node_lookup(children[i], index);
            }
            index -= children[i]->size;
        }
        return NULL;
    }
}

void* affix_lookup(affix* a, int index) {
    for (int i = 0; i < 4; i++) {
        if (a->nodes[i] == NULL)
            continue;
        
        if (index < a->nodes[i]->size) {
            return node_lookup(a->nodes[i], index);
        }
        index -= a->nodes[i]->size;
    }
    
    return NULL;
}

void* ft_lookup(ft* ft, int index) {
    // Preconditions & Invariants
    checkInvariants(ft);
    
    if (ft->type == EMPTY_TYPE) {
        return NULL;
    }
    else if (ft->type == SINGLE_TYPE) {
        node *n = ft->true_ft->single;
        
        if (index > n->size)
            return NULL;
        
        node_lookup(n, index);
    }
    else if (ft->type == DEEP_TYPE) {
        deep* d = ft->true_ft->d;
        // Prefix
        if (index < d->prefix->size) {
            return affix_lookup(d->prefix, index);
        }
        index -= d->prefix->size;
        // Deeper
        if (index < d->deeper->size) {
            return ft_lookup(d->deeper, index);
        }
        index -= d->deeper->size;
        
        // Suffix
        if (index < d->suffix->size) {
            return affix_lookup(d->suffix, index);
        }
        return NULL;
    }
    
    // Postconditions & Invariants
    
    return NULL;
}

void node_unref(node* n) {
    if (n == NULL)
        return;
    
    n->ref_count--;
    
    if (n->ref_count == 0) {
        if (n->type == DATA_TYPE) {
            free(n->true_node);
            free(n);
        }
        else {
            for (int i = 0; i < 3; i++) {
                if (n->true_node->internal_node[i] == NULL)
                    continue;
                node_unref(n->true_node->internal_node[i]);
            }
            free(n->true_node);
            free(n);
        }
    }
}

void ft_unref_rec(ft* ft) {
    ft->ref_count--;
    
    if (ft->ref_count == 0) {
        if (ft->type == EMPTY_TYPE) {
            free(ft);
        }
        else if (ft->type == SINGLE_TYPE) {
            node_unref(ft->true_ft->single);
            free(ft->true_ft);
            free(ft);
        }
        else {
            // Prefix
            for (int i = 0; i < 4; i++) {
                if(ft->true_ft->d->prefix->nodes[i] == NULL)
                    continue;
                
                node_unref(ft->true_ft->d->prefix->nodes[i]);
            }
            free(ft->true_ft->d->prefix);
            
            // Deep
            ft_unref_rec(ft->true_ft->d->deeper);
            
            // Suffix
            for (int i = 0; i < 4; i++) {
                if(ft->true_ft->d->suffix->nodes[i] == NULL)
                    continue;
                
                node_unref(ft->true_ft->d->suffix->nodes[i]);
            }
            free(ft->true_ft->d->suffix);
            free(ft->true_ft->d);
            free(ft->true_ft);
            free(ft);
        }
    }
}

void ft_unref(ft* ft) {
    // Preconditions & Invariants
    checkInvariants(ft);
       ft_unref_rec(ft);
   
    // Postconditions & Invariants
    
}

view ft_delete_rec(ft* fgt, int preorsuf) {
    ft* res=NULL;
    view stres;
    stres.elem=NULL;
    stres.fg=NULL;
    affix*old_affix;
    affix*new_affix;
    if(fgt->type==EMPTY_TYPE){
        /* It might be better to throw an exception see with Hugo*/
        res = create_empty();
        stres.fg=res;
        stres.elem=NULL;
    }
    else if(fgt->type == SINGLE_TYPE) {
        res = create_empty();
        stres.fg=res;
        stres.elem=fgt->true_ft->single;
    }
    else{
        if(preorsuf) {
            stres.elem = fgt->true_ft->d->suffix->nodes[3];
        } else {
            stres.elem = fgt->true_ft->d->prefix->nodes[0];
        }
        int index = check_available_space(fgt,preorsuf);
        if((preorsuf==0 && index!=1) || (preorsuf==1 && index!=2)) {
            /* the resulting tree is of type deep*/
            res = create_deep_withoutdeeper();
            if (index==-1) {
                if(preorsuf)index=0;
                else index=3;
            }
            /* lets simply remove an element from the affix*/
            copy_pref(res,fgt);
            copy_suff(res,fgt);
            new_affix=get_right_affix(res,preorsuf,0);
            old_affix=get_right_affix(fgt,preorsuf,0);
            res->true_ft->d->deeper = fgt->true_ft->d->deeper;
            fgt->true_ft->d->deeper->ref_count++;
            remove_from_affix(new_affix,old_affix,index,preorsuf);
            res->size = res->true_ft->d->prefix->size + res->true_ft->d->suffix->size + res->true_ft->d->deeper->size;
	    stres.elem->ref_count--;
        }
        else{
            /* there is only one element in the affix*/
            if(fgt->true_ft->d->deeper->type==EMPTY_TYPE){
                /* the deeper is empty
                 we look the other side and see if we can take one element*/
                int inv;
                if(preorsuf)inv=0;else inv=1;
                index = check_available_space(fgt,inv);
                if((preorsuf==1 && index!=1) || (preorsuf==0 && index!=2)){
                    // We take an element from the other side
                    if(index==-1) {
                        if(preorsuf) index=3;
                        else index=0;
                    }
                    else {
                        if(preorsuf) index--;
                        else  index++;
                    }
                    
                    res=create_deep_withoutdeeper();
		    res->true_ft->d->deeper = fgt->true_ft->d->deeper;
		    fgt->true_ft->d->deeper->ref_count++;
		    
                    new_affix=get_right_affix(res,preorsuf,0);
                    old_affix=get_right_affix(fgt,preorsuf,1);
                    if(preorsuf)new_affix->nodes[3]=old_affix->nodes[index];
                    else new_affix->nodes[0]=old_affix->nodes[index];
                  
                    new_affix->size = old_affix->nodes[index]->size;
                    if(preorsuf) {
                        copy_pref(res,fgt);
                    }
                    else {
                        copy_suff(res,fgt);
                    }
                    new_affix=get_right_affix(res,preorsuf,1);
                    new_affix->size -= new_affix->nodes[index]->size;
                    new_affix->nodes[index]=NULL;
                    res->size = res->true_ft->d->prefix->size + res->true_ft->d->suffix->size + res->true_ft->d->deeper->size;
                    
                    
                }
                else{
                    // There is only 1 element on the other side
                    // We put it in a Single
                    old_affix=get_right_affix(fgt,preorsuf,1);
                    if(!preorsuf){
                        
                        res=create_single(old_affix->nodes[3]);
                        old_affix->nodes[3]->ref_count++;
                        
                    }
                    else{
                        res=create_single(old_affix->nodes[0]);
                        old_affix->nodes[0]->ref_count++;
                    }
                }
            }
            else if(fgt->true_ft->d->deeper->type==SINGLE_TYPE){
                // The deeper is of type Single
                node**tmp;
                res = create_deep();
                new_affix=get_right_affix(res,preorsuf,0);
                if(preorsuf) {
                    copy_pref(res,fgt);
                } else {
                    copy_suff(res,fgt);
                }
                tmp = fgt->true_ft->d->deeper->true_ft->single->
                true_node->internal_node;
                int i,j;
                if(preorsuf){
                    for(i=0;i<3;i++){
                        j=i+1;
                        if (tmp[i] == NULL) break;
                        new_affix->size += tmp[i]->size;
                        new_affix->nodes[j]=tmp[i];
                        tmp[i]->ref_count++;
                    }
                    
                }
                else{
                    
                    for(i=0;i<3;i++){
                        j=i;
                        if (tmp[i] == NULL) break;
                        new_affix->size += tmp[i]->size;
                        new_affix->nodes[j]=tmp[i];
                        tmp[i]->ref_count++;
                    }
                }
                res->size = res->true_ft->d->prefix->size + res->true_ft->d->suffix->size + res->true_ft->d->deeper->size;
            }
            else{
                /* the deeper is of type deep*/
                node** getres;
                res = create_deep_withoutdeeper();
                if(preorsuf) {
                    copy_pref(res,fgt);
                } else {
                    copy_suff(res,fgt);
                }
                view tmp =ft_delete_rec(fgt->true_ft->d->deeper,preorsuf);
                new_affix=get_right_affix(res,preorsuf,0);
                getres = (tmp.elem)->true_node->internal_node;
                if(preorsuf){
                    int j;
                    for(int i=0;i<3;i++){
                        j=i+1;
                        if (getres[i] == NULL) break;
                        new_affix->size += getres[i]->size;
                        new_affix->nodes[j]=getres[i];
                        getres[i]->ref_count++;
                    }
                }
                else{
                    for(int i=0;i<3;i++){
                        
                        if (getres[i] == NULL) break;
                        
                        new_affix->size += getres[i]->size;
                        new_affix->nodes[i]=getres[i];
                        getres[i]->ref_count++;
                    }
                    
                }
                res->true_ft->d->deeper= tmp.fg;
                res->size = res->true_ft->d->prefix->size + res->true_ft->d->suffix->size + res->true_ft->d->deeper->size;
            }
        }
    }
    stres.fg=res;
    return stres;
}

view ft_delete(ft* fgt,int preorsuf){
    // Preconditions & Invariants
    checkInvariants(fgt);
    // Save the reference counter of nodes and deeper of fgt.
    reflist* rl_fgt = rl_of_ft(fgt);
    refdeep rd_fgt, rd_res;
    if (fgt->type == DEEP_TYPE && fgt->true_ft->d->deeper != NULL) {
        rd_fgt.elem = fgt->true_ft->d->deeper;
        rd_fgt.ref = rd_fgt.elem->ref_count;
    }
    else {
        rd_fgt.elem = NULL;
        rd_fgt.ref = 0;
    }
    
    view stres = ft_delete_rec(fgt, preorsuf);
    
    // Postconditions & Invariants
    checkInvariants(stres.fg);
    
    // Compare the reference counters of nodes and deeper of fgt (before the function) and res
    reflist* rl_res = rl_of_ft(stres.fg);
    inter_verify(rl_fgt, rl_res);
    if (stres.fg->type == DEEP_TYPE && stres.fg->true_ft->d->deeper != NULL) {
        rd_res.elem = stres.fg->true_ft->d->deeper;
        rd_res.ref = rd_res.elem->ref_count;
        
        // Compare rd_res and rd_fgt
        if (rd_fgt.elem != NULL) {
            if (rd_fgt.elem == rd_res.elem) {
                assert(rd_fgt.ref + 1 >= rd_res.ref);
            }
        }
    }
    free_reflist(rl_fgt);
    free_reflist(rl_res);
    
    return stres;
}



// Split and its helper functions
// conversion functions leave the ref_count untouched.
void copy_affix(affix* src, affix* dest, int incr){
    for(int i = 0; i < 4; i++){
      dest->nodes[i] = src->nodes[i];
      if(incr == 1 && src->nodes[i] != NULL) {
	      src->nodes[i]->ref_count++;
      }
    }
    
    dest->size = src->size;
}

ft* create_deep_withprefix() {
    ft* fgt = malloc(sizeof(ft));
    fgt->type = DEEP_TYPE;
    fgt->true_ft = malloc(sizeof(true_ft_t));
    fgt->true_ft->d = malloc(sizeof(deep));
    
    fgt->true_ft->d->prefix = malloc(sizeof(affix));
    for(int i=0;i<4;i++){
        fgt->true_ft->d->prefix->nodes[i] = NULL;
    }
    fgt->true_ft->d->prefix->size = 0;
    
    fgt->ref_count = 1;
    fgt->size = 0; // The caller of this function should put the right size.
    
    return fgt;
}

ft* create_deep_withsuffix() {
    ft* fgt = malloc(sizeof(ft));
    fgt->type = DEEP_TYPE;
    fgt->true_ft = malloc(sizeof(true_ft_t));
    fgt->true_ft->d = malloc(sizeof(deep));
    
    fgt->true_ft->d->suffix = malloc(sizeof(affix));
    for(int i=0;i<4;i++){
        fgt->true_ft->d->suffix->nodes[i] = NULL;
    }
    fgt->true_ft->d->suffix->size = 0;
    
    fgt->ref_count = 1;
    fgt->size = 0; // The caller of this function should put the right size.
    
    return fgt;
}

ft* create_deep_withoutaffix() {
    ft* fgt = malloc(sizeof(ft));
    fgt->type = DEEP_TYPE;
    fgt->true_ft = malloc(sizeof(true_ft_t));
    fgt->true_ft->d = malloc(sizeof(deep));
    
    fgt->true_ft->d->deeper = create_empty();
    fgt->ref_count = 1;
    fgt->size = 0; // The caller of this function should put the right size.
    
    return fgt;
}

ft* nodearray_to_ft(node* arr[], int size) {
    // Precondition: 1 <= size <= 4
    ft* res;
    deep *d;
    
    switch (size) {
        case 1:
            res = create_single(arr[0]);
            //arr[0]->ref_count++;
            break;
        case 2:
            res = create_deep();
            d = res->true_ft->d;
            d->prefix->nodes[0] = arr[0];
            d->prefix->size = arr[0]->size;
            d->suffix->nodes[3] = arr[1];
            d->suffix->size = arr[1]->size;
            res->size = d->prefix->size + d->suffix->size;
            /*arr[0]->ref_count++;
            arr[1]->ref_count++;*/
            break;
        case 3: // case 3
            res = create_deep();
            d = res->true_ft->d;
            d->prefix->nodes[0] = arr[0];
            d->prefix->size = arr[0]->size;
            d->suffix->nodes[2] = arr[1];
            d->suffix->nodes[3] = arr[2];
            d->suffix->size = arr[1]->size + arr[2]->size;
            res->size = d->prefix->size + d->suffix->size;
            /*arr[0]->ref_count++;
            arr[1]->ref_count++;
            arr[2]->ref_count++;*/
            break;
        default: // case 4
            res = create_deep();
            d = res->true_ft->d;
            d->prefix->nodes[0] = arr[0];
            d->prefix->size = arr[0]->size;
            d->suffix->nodes[1] = arr[1];
            d->suffix->nodes[2] = arr[2];
            d->suffix->nodes[3] = arr[3];
            d->suffix->size = arr[1]->size + arr[2]->size + arr[3]->size;
            res->size = d->prefix->size + d->suffix->size;;
            /*arr[0]->ref_count++;
            arr[1]->ref_count++;
            arr[2]->ref_count++;
            arr[3]->ref_count++;*/
            break;
    }
    
    return res;
}

void affix_to_nodearray(affix* a, node** nodes) {
    for (int i = 0, j = 0; i < 4; i++) {
        if (a->nodes[i] != NULL) {
            nodes[j] = a->nodes[i];
            //a->nodes[i]->ref_count++;
            j++;
        }
    }
}

affix* node_to_affix(node* n, int preorsuf) {
    // The node has 2 or 3 children
    // Returns an affix of 2 or 3 elements
    affix* res = malloc(sizeof(affix));
    node** nodes = n->true_node->internal_node;
    int i, j;
    
    if (preorsuf == 0) {
        for (i = 0, j = 0; i < 3; i++) {
            if (n->true_node->internal_node[i] != NULL) {
                res->nodes[j++] = nodes[i];
                nodes[i]->ref_count++;
                res->size += nodes[i]->size;
            }
        }
        while (j <= 3) {
            res->nodes[j++] = NULL;
        }
    }
    else {
        for (i = 2, j = 3; i >= 0; i--) {
            if (n->true_node->internal_node[i] != NULL) {
                res->nodes[j--] = nodes[i];
                nodes[i]->ref_count++;
                res->size += nodes[i]->size;
            }
        }
        while (j >= 0) {
            res->nodes[j--] = NULL;
        }
    }
    
    return res;
}

ft* borrowL(ft* deeper, affix* su) {
    ft* res;
    node* nodes[4];
    int node_count = 0;
    
    for (int i = 0; i < 4; i++) {
        if (su->nodes[i] != NULL) {
            node_count++;
            su->nodes[i]->ref_count++;
        }
    }
    
    affix_to_nodearray(su, nodes);
    
    if (deeper->type == EMPTY_TYPE) {
        res = nodearray_to_ft(nodes, node_count);
    }
    else {
        view v = ft_delete_rec(deeper, 0);
        res = create_deep_withsuffix();
        res->true_ft->d->prefix = node_to_affix(v.elem, 0);
        res->true_ft->d->deeper = v.fg;
        copy_affix(su, res->true_ft->d->suffix, 0);
        deep* d = res->true_ft->d;
        res->size = d->prefix->size + d->deeper->size + d->suffix->size;
    }
    
    return res;
}

ft* create_deepL(affix* left, ft* deeper, affix* right) {
    // Creates a deep with the given elements. left can be NULL.
    if (left == NULL)
        return borrowL(deeper, right);
    
    ft* res;
    
    res = create_deep_withoutdeeper();
    copy_affix(left, res->true_ft->d->prefix, 1);
    deeper->ref_count++;
    res->true_ft->d->deeper = deeper;
    copy_affix(right, res->true_ft->d->suffix, 1);
    deep* d = res->true_ft->d;
    res->size = d->prefix->size + d->deeper->size + d->suffix->size;
    
    return res;
}

ft* borrowR(affix* pr, ft* deeper) {
    ft* res;
    node* nodes[4];
    int node_count = 0;
    for (int i = 0; i < 4; i++) {
        if (pr->nodes[i] != NULL) {
            node_count++;
            pr->nodes[i]->ref_count++;   
        }
    }
    affix_to_nodearray(pr, nodes);
    
    if (deeper->type == EMPTY_TYPE) {
        res = nodearray_to_ft(nodes, node_count);
    }
    else {
        view v = ft_delete_rec(deeper, 1);
        res = create_deep_withprefix();
        copy_affix(pr, res->true_ft->d->prefix, 0);
        res->true_ft->d->deeper = v.fg;
        res->true_ft->d->suffix = node_to_affix(v.elem, 1);
        deep* d = res->true_ft->d;
        res->size = d->prefix->size + d->deeper->size + d->suffix->size;
    }
    
    return res;
}

ft* create_deepR(affix* left, ft* deeper, affix* right) {
    // Creates a deep with the given elements. right can be NULL.
    if (right == NULL)
        return borrowR(left, deeper);
    
    ft* res = create_deep_withoutdeeper();
    
    copy_affix(left, res->true_ft->d->prefix, 1);
    deeper->ref_count++;
    res->true_ft->d->deeper = deeper;
    copy_affix(right, res->true_ft->d->suffix, 1);
    deep* d = res->true_ft->d;
    res->size = d->prefix->size + d->deeper->size + d->suffix->size;
    
    return res;
}

affix* ft_to_affix(ft* fgt, int preorsuf) {
    // fgt is either
    // - an Empty
    // - a Single
    // - a Deep([Node], Empty, [Node])
    // - a Deep([Node], Empty, [Node,Node])
    if (fgt->type == EMPTY_TYPE)
        return NULL;
    
    affix* res = malloc(sizeof(affix));
    
    if (preorsuf == 0) {
        // We make a prefix
        if (fgt->type == SINGLE_TYPE) {
            res->nodes[0] = fgt->true_ft->single;
            res->nodes[0]->ref_count++;
            for (int i = 1; i < 4; i++) {
                res->nodes[i] = NULL;
            }
            res->size = fgt->true_ft->single->size;
        }
        else {
            if (fgt->true_ft->d->suffix->nodes[2] == NULL) {
                // 1 Node in the suffix
                res->nodes[0] = fgt->true_ft->d->prefix->nodes[0];
                res->size += fgt->true_ft->d->prefix->nodes[0]->size;
                res->nodes[1] = fgt->true_ft->d->suffix->nodes[3];
                res->size += fgt->true_ft->d->suffix->nodes[3]->size;
                res->nodes[0]->ref_count++;
                res->nodes[1]->ref_count++;
                for (int i = 2; i < 4; i++) {
                    res->nodes[i] = NULL;
                }
            }
            else {
                // 2 Node in the suffix
                res->nodes[0] = fgt->true_ft->d->prefix->nodes[0];
                res->size += fgt->true_ft->d->prefix->nodes[0]->size;
                res->nodes[1] = fgt->true_ft->d->suffix->nodes[2];
                res->size += fgt->true_ft->d->suffix->nodes[2]->size;
                res->nodes[2] = fgt->true_ft->d->suffix->nodes[3];
                res->size += fgt->true_ft->d->suffix->nodes[3]->size;
                res->nodes[0]->ref_count++;
                res->nodes[1]->ref_count++;
                res->nodes[2]->ref_count++;
                res->nodes[3] = NULL;
            }
        }
    }
    else {
        // We make a suffix
        if (fgt->type == SINGLE_TYPE) {
            res->nodes[3] = fgt->true_ft->single;
            res->size = fgt->true_ft->single->size;
            res->nodes[3]->ref_count++;
            for (int i = 0; i < 3; i++) {
                res->nodes[i] = NULL;
            }
        }
        else {
            if (fgt->true_ft->d->suffix->nodes[2] == NULL) {
                // 1 Node in the suffix
                res->nodes[2] = fgt->true_ft->d->prefix->nodes[0];
                res->size += fgt->true_ft->d->prefix->nodes[0]->size;
                res->nodes[3] = fgt->true_ft->d->suffix->nodes[3];
                res->size += fgt->true_ft->d->suffix->nodes[3]->size;
                res->nodes[2]->ref_count++;
                res->nodes[3]->ref_count++;
                for (int i = 0; i < 2; i++) {
                    res->nodes[i] = NULL;
                }
            }
            else {
                // 2 Node in the suffix
                res->nodes[1] = fgt->true_ft->d->prefix->nodes[0];
                res->size += fgt->true_ft->d->prefix->nodes[0]->size;
                res->nodes[2] = fgt->true_ft->d->suffix->nodes[2];
                res->size += fgt->true_ft->d->suffix->nodes[2]->size;
                res->nodes[3] = fgt->true_ft->d->suffix->nodes[3];
                res->size += fgt->true_ft->d->suffix->nodes[3]->size;
                res->nodes[1]->ref_count++;
                res->nodes[2]->ref_count++;
                res->nodes[3]->ref_count++;
                res->nodes[0] = NULL;
            }
        }
    }
    
    return res;
}


split affix_split(affix* a, int index) {
    split s;
    int node_count = 0;
    for (int i = 0; i < 4; i++) {
        if (a->nodes[i] != NULL)
            node_count++;
    }
    
    // The "nodes" array is filled starting from 0 in order,
    // whereas a->nodes might not.
    node* nodes[4];
    affix_to_nodearray(a, nodes);
    
    switch (node_count) {
        case 1:
            s.ft1 = create_empty();
            s.node = nodes[0];
            s.ft2 = create_empty();
            nodes[0]->ref_count++;
            break;
        case 2:
            if (index < nodes[0]->size) {
                s.ft1 = create_empty();
                s.node = nodes[0];
                s.ft2 = create_single(nodes[1]);
            }
            else {
                s.ft1 = create_single(nodes[0]);
                s.node = nodes[1];
                s.ft2 = create_empty();
            }
            nodes[0]->ref_count++;
            nodes[1]->ref_count++;
            break;
        case 3:
            if (index < nodes[0]->size) {
                s.ft1 = create_empty();
                s.node = nodes[0];
                s.ft2 = nodearray_to_ft(nodes+1, 2);
            }
            else if (index < nodes[0]->size+nodes[1]->size) {
                s.ft1 = create_single(nodes[0]);
                s.node = nodes[1];
                s.ft2 = create_single(nodes[2]);
            }
            else {
                s.ft1 = nodearray_to_ft(nodes, 2);
                s.node = nodes[2];
                s.ft2 = create_empty();
            }
            nodes[0]->ref_count++;
            nodes[1]->ref_count++;
            nodes[2]->ref_count++;
            break;
        default: // case 4
            if (index < nodes[0]->size) {
                s.ft1 = create_empty();
                s.node = nodes[0];
                s.ft2 = nodearray_to_ft(nodes+1, 3);
            }
            else if (index < nodes[0]->size+nodes[1]->size) {
                s.ft1 = create_single(nodes[0]);
                s.node = nodes[1];
                s.ft2 = nodearray_to_ft(nodes+2, 2);
            }
            else if (index < nodes[0]->size+nodes[1]->size+nodes[2]->size) {
                s.ft1 = nodearray_to_ft(nodes, 2);
                s.node = nodes[2];
                s.ft2 = create_single(nodes[3]);
            }
            else {
                s.ft1 = nodearray_to_ft(nodes, 3);
                s.node = nodes[3];
                s.ft2 = create_empty();
            }
            nodes[0]->ref_count++;
            nodes[1]->ref_count++;
            nodes[2]->ref_count++;
            nodes[3]->ref_count++;
            break;
    }
    
    return s;
}

splitnode node_split(node* n, int index) {
    // Caution:
    // sn.left is to be used as a suffix, so it is filled from the right
    // sn.right is to be used as a prefix, so it is filled from the left
    splitnode sn;
    
    node* nodes[3];
    for (int i = 0, j = 0; i < 3; i++) {
        if (n->true_node->internal_node[i] != NULL) {
            nodes[j] = n->true_node->internal_node[i];
            j++;
        }
    }
    
    int node_count = 0;
    for (int i = 0; i < 3; i++) {
        if (nodes[i] != NULL)
            node_count++;
    }
    
    // node_count is either 2 or 3
    
    if (index < nodes[0]->size) {
        sn.left = NULL;
        sn.node = nodes[0];
        sn.right = malloc(sizeof(affix));
        if (node_count == 2) {
            sn.right->nodes[0] = nodes[1];
            sn.right->nodes[1] = NULL;
            sn.right->size = nodes[1]->size;
        }
        else {
            sn.right->nodes[0] = nodes[1];
            sn.right->nodes[1] = nodes[2];
            sn.right->size = nodes[1]->size+nodes[2]->size;
        }
        sn.right->nodes[2] = NULL;
        sn.right->nodes[3] = NULL;
        
    }
    else if (index < nodes[0]->size+nodes[1]->size) {
        sn.left = malloc(sizeof(affix));
        sn.left->nodes[0] = NULL;
        sn.left->nodes[1] = NULL;
        sn.left->nodes[2] = NULL;
        sn.left->nodes[3] = nodes[0];
        sn.left->size = nodes[0]->size;
        sn.node = nodes[1];
        if (node_count == 2) {
            sn.right = NULL;
        }
        else {
            sn.right = malloc(sizeof(affix));
            sn.right->nodes[0] = nodes[2];
            sn.right->nodes[1] = NULL;
            sn.right->nodes[2] = NULL;
            sn.right->nodes[3] = NULL;
            sn.right->size = nodes[2]->size;
        }
    }
    else {
        sn.left = malloc(sizeof(affix));
        sn.left->nodes[0] = NULL;
        sn.left->nodes[1] = NULL;
        sn.left->nodes[2] = nodes[0];
        sn.left->nodes[3] = nodes[1];
        sn.left->size = nodes[0]->size+nodes[1]->size;
        sn.node = nodes[2];
        sn.right = NULL;
    }
    
    return sn;
}

split ft_split_rec(ft* fgt, int index) {
    split s;
    
    if (fgt->type == SINGLE_TYPE) {
        s.ft1 = create_empty();
        s.ft2 = create_empty();
        s.node = fgt->true_ft->single;
        fgt->true_ft->single->ref_count++;
    }
    else {
        deep *d = fgt->true_ft->d;
        split recs;
        if (index < d->prefix->size) {
            // The node is in the prefix
            recs = affix_split(d->prefix, index);
            
            s.ft1 = recs.ft1;
            s.node = recs.node;
            s.ft2 = create_deepL(ft_to_affix(recs.ft2, 1), d->deeper, d->suffix);
            ft_unref(recs.ft2);
        }
        else if (index >= d->prefix->size + d->deeper->size) {
            // The node is in the suffix
            recs = affix_split(d->suffix, index-(d->prefix->size+d->deeper->size));
            s.ft1 = create_deepR(d->prefix, d->deeper, ft_to_affix(recs.ft1, 1));
            s.node = recs.node;
            s.ft2 = recs.ft2;
            
            ft_unref(recs.ft1);
        }
        else {
            // The node is in the deeper
            recs = ft_split_rec(d->deeper, index - d->prefix->size);
            splitnode recsn = node_split(recs.node, index - d->prefix->size - recs.ft1->size);
            s.ft1 = create_deepR(d->prefix, recs.ft1, recsn.left);
            s.node = recsn.node;
            s.node->ref_count++;
            s.ft2 = create_deepL(recsn.right, recs.ft2, d->suffix);
            ft_unref(recs.ft1);
            node_unref(recs.node);
            if (recsn.left != NULL)
              free(recsn.left);
            if (recsn.right != NULL)
              free(recsn.right);
            ft_unref(recs.ft2);
        }
    }
    
    return s;
}

split ft_split(ft* fgt, int index) {
    // Preconditions & Invariants
    checkInvariants(fgt);
    assert(fgt->type != EMPTY_TYPE);
    split s = ft_split_rec(fgt, index);
    checkInvariants(s.ft1);
    checkInvariants(s.ft2);
    
    return s;
}


ft* concat_w_middle(ft* ft1, list* l,ft* ft2){
    ft* res;
    node* x;
    /*we treat the different base cases first*/
    if(ft1->type==EMPTY_TYPE && l==NULL)
        return ft2;
    else if(ft1->type==EMPTY_TYPE){
        x = first(l);
        res = concat_w_middle(ft1,removel(l),ft2);
        res =  add_elem_deep_recur(res,0,x);
    }
    else if(ft1->type==SINGLE_TYPE){
        ft* tmp = create_empty();
        res = concat_w_middle(tmp,l,ft2);
        res =  add_elem_deep_recur(res,0,ft1->true_ft->single);

    }
    else if(l==NULL && ft2->type==EMPTY_TYPE){
        return ft1;
    }
    else if(ft2->type==EMPTY_TYPE){
        node* x = last(l);
        res = concat_w_middle(ft1,remove_last(l),ft2);
        res =  add_elem_deep_recur(res,1,x);
    }
    else if(ft2->type==SINGLE_TYPE){
        ft* tmp = create_empty();
        res = concat_w_middle(ft1,l,tmp);
        res =  add_elem_deep_recur(res,1,ft2->true_ft->single);
    } else{
        res = create_deep();
        /*copy prefix*/
        copy_pref(res,ft1);
        /*copy suffix*/
        copy_suff(res,ft2);

        ft*deeper_tmp=create_empty();
        /* nodes on list*/
        list* l1 = affix_to_list(ft1,1);
        list* l2 = affix_to_list(ft2,0);
        l = concat(l1,l);
        l = concat(l,l2);
        l = nodes(l);
        deeper_tmp = concat_w_middle(ft1->true_ft->d->deeper,l,ft2->true_ft->d->deeper);
        res->true_ft->d->deeper = deeper_tmp;
	res->size = deeper_tmp->size + res->true_ft->d->prefix->size + res->true_ft->d->suffix->size;
    }
    return res;
}




ft* ft_generator(int nbelem,int* elems){
  srand((unsigned int)time(NULL));
 ft* tab[nbelem+1];
 int r,i=1;
 tab[0]= create_empty();
 while(i<=nbelem){
   r = rand()%2;
   tab[i] = ft_add(&elems[i-1],tab[i-1],r);
   i++;

 }
 for(int j=0;j<nbelem;j++)
   ft_unref(tab[j]);
 return tab[nbelem];
 
}



void simulation_add(int density_each_point, int shift, int starting_size, int finishing_size){
  double time_spent;
  double average;
  /*open the file */
  char filename[100];
  FILE* f;
  sprintf(filename,"add_density:%d_shift:%d_start:%d_fin:%d",density_each_point,shift,starting_size,finishing_size);
  f = fopen(filename,"w+");
  /* initializing data*/
  int data[finishing_size];
  for(int i=0;i<finishing_size;i++)
    data[i]=1;

  ft* fin[density_each_point];
  ft*tmp=NULL;
  for(int i=starting_size;i<=finishing_size;i=i+shift){
    average = 0;
    for(int j=0;j<density_each_point;j++){
      
      fin[j]=ft_generator(i,data);
      clock_t begin = clock();
      tmp = ft_add(&data[0],fin[j],0);
       clock_t end = clock();
     time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
     average = average + time_spent;
     ft_unref(tmp);
      
    }
    average = average/density_each_point;
    fprintf(f,"%d %g\n",i,average*1000000);

  }
  fclose(f);
}
