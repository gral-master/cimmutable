#include <stdio.h>
#include <stdlib.h>
#include "fingertree.h"
#include "invariants.h"
#include "list.h"

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
    fgt->size = 1;
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
            printf("%d", *((int*)node->true_node->data));
            break;
        case NODE_TYPE:
            printf("Node(");
            for (int i = 0; i < 3; i++){
                if (node->true_node->internal_node[i] != NULL) {
                    node_display(node->true_node->internal_node[i]);
                    printf(",");
                }
            }
            printf(")");
            break;
        default:
            printf("_");
    }
}

void ft_display(ft* fgt) {
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
                    node_display(fgt->true_ft->d->prefix->nodes[i]);
                    printf(",");
                }
            }
            printf("],");
            // Deeper
            ft_display(fgt->true_ft->d->deeper);
            // Suffix
            printf(",[");
            for (int i = 3; i >=0; i--){
                if (fgt->true_ft->d->suffix->nodes[i] != NULL){
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

/*
* this function is recursive. it builds the resulting finger tree
* the recursivity occurs when there is no space left in the prefix or suffix
*/
ft* add_elem_deep_recur(ft* fgt,int preorsuf,node*data_node){
    ft*res;
    affix*old_affix;
    affix*new_affix;
    int i;
    if(fgt->type==EMPTY_TYPE){
        res = create_single(data_node);
        res->size = data_node->size;
    }
    else if(fgt->type == SINGLE_TYPE) {
        res = create_deep();
        if(preorsuf){
            old_affix=res->true_ft->d->suffix;
            new_affix=res->true_ft->d->prefix;
        }
        else{
            new_affix=res->true_ft->d->suffix;
            old_affix=res->true_ft->d->prefix;
        }
        fgt->true_ft->single->ref_count++;
        new_affix->nodes[0] = fgt->true_ft->single;
        old_affix->nodes[0] = data_node;
        new_affix->size = fgt->true_ft->single->size;
        old_affix->size = data_node->size;
        res->size = new_affix->size + old_affix->size;
    }
    else {
        res = create_deep();
        int index=check_available_space(fgt,preorsuf);

        /* copy the pref and suff*/
        copy_pref(res,fgt);
        copy_suff(res,fgt);

        new_affix=get_right_affix(res,preorsuf,0);
        old_affix=get_right_affix(fgt,preorsuf,0);

        if(index==-1) {
            // There is not enough space in the affix
            node* new_node = create_node_node();
            int i;
            for(i=0;i<3;i++){
                new_node->true_node->internal_node[i] = old_affix->nodes[i+1];
                new_node->size += old_affix->nodes[i+1]->size;
            }
            /* creating the new suffix or prefix*/
            new_affix->nodes[0]=data_node;
            new_affix->nodes[1]=old_affix->nodes[0];
            new_affix->nodes[2]=NULL;
            new_affix->nodes[3]=NULL;
            new_affix->size=data_node->size + old_affix->nodes[0]->size;
            /* recursive call to the same function with deeper*/
            res->true_ft->d->deeper=add_elem_deep_recur(fgt->true_ft->d->deeper,
                preorsuf,new_node);

            res->size = res->true_ft->d->deeper->size + res->true_ft->d->prefix->size+res->true_ft->d->suffix->size;
        }
        else {
            /* we can add directly, there is space left in prefix or suffix*/
            res->true_ft->d->deeper = fgt->true_ft->d->deeper;
            fgt->true_ft->d->deeper->ref_count++;

            for(i=index-1;i>=0;i--) {
                new_affix->nodes[i+1]=new_affix->nodes[i];
            }
            /* add the new element*/
            new_affix->nodes[0]=data_node;

            new_affix->size += data_node->size;

            res->size = res->true_ft->d->deeper->size + res->true_ft->d->prefix->size+res->true_ft->d->suffix->size;
        }
    }

    return res;
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
    affix*old_affix;
    if(preorsuf){
        old_affix = fgt->true_ft->d->suffix;
    }else{
        old_affix = fgt->true_ft->d->prefix;
    }

    int i, j = old_affix->size;

    for (i = 0; i < 4; i++) {
        if (j == 0)
            break;
        j -= old_affix->nodes[i]->size;
    }

    if (i > 3) {
        return -1;
    }

    return i;
}

void* node_lookup(node* n, int index) {
    if (n->type == DATA_TYPE)
        return n->true_node->data;
    else {
        node** children = n->true_node->internal_node;
        
        for (int i = 0; i < 3; i++) {
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

view ft_delete(ft* fgt,int preorsuf){
    // Preconditions & Invariants
    checkInvariants(fgt);
    
    ft* res;
    view stres;
    affix*old_affix;
    affix*new_affix;
    if(fgt->type==EMPTY_TYPE){
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
            stres.elem = fgt->true_ft->d->suffix->nodes[0];
        } else {
            stres.elem = fgt->true_ft->d->prefix->nodes[0];
        }

        int index = check_available_space(fgt,preorsuf);
        if(index==-1 || index>1) {
            int i;
            /* the resulting tree is of type deep*/
            res = create_deep();
            if (index==-1) {
                index=3;
            }
            /* lets simply remove an element from the affix*/
            copy_pref(res,fgt);
            copy_suff(res,fgt);
            new_affix=get_right_affix(res,preorsuf,0);
            old_affix=get_right_affix(fgt,preorsuf,0);
            res->true_ft->d->deeper = fgt->true_ft->d->deeper;
            fgt->true_ft->d->deeper->ref_count++;
            new_affix->size = old_affix->size - new_affix->nodes[0]->size;
            for(i=0;i<index;i++) {
                new_affix->nodes[i]=new_affix->nodes[i+1];
            }
            new_affix->nodes[index]=NULL;
            
            res->size = res->true_ft->d->prefix->size + res->true_ft->d->suffix->size + res->true_ft->d->deeper->size;
        } else {
            /* there is only one element in the affix*/
            if(fgt->true_ft->d->deeper->type==EMPTY_TYPE){
                /* the deeper is empty
                we look the other side and see if we can take one element*/
                int inv;
                if(preorsuf)inv=0;else inv=1;
                index = check_available_space(fgt,inv);
                if(index!=1){
                    // We take an element from the other side
                    if(index==-1) {
                        index=3;
                    } else {
                        index--;
                    }
                    res=create_deep();
                    new_affix=get_right_affix(res,preorsuf,0);
                    old_affix=get_right_affix(fgt,preorsuf,1);
                    new_affix->nodes[0]=old_affix->nodes[index];
                    new_affix->size = old_affix->nodes[index]->size;
                    
                    old_affix->nodes[index]->ref_count++;
                    if(preorsuf) {
                        copy_pref(res,fgt);
                    } else {
                        copy_suff(res,fgt);
                    }
                    new_affix=get_right_affix(res,preorsuf,1);
                    new_affix->size -= new_affix->nodes[index]->size;
                    new_affix->nodes[index]=NULL;
                    res->size = res->true_ft->d->prefix->size + res->true_ft->d->suffix->size + res->true_ft->d->deeper->size;
                } else{
                    // There is only 1 element on the other side
                    // We put it in a Single
                    old_affix=get_right_affix(fgt,preorsuf,1);
                    res=create_single(old_affix->nodes[0]);
                    old_affix->nodes[0]->ref_count++;
                    
                    res->size = old_affix->nodes[0]->size;
                }
            }
            else if(fgt->true_ft->d->deeper->type==SINGLE_TYPE){
                // The deeper is of type Single
                node**tmp;
                res = create_deep();
                res->true_ft->d->deeper=create_empty();
                new_affix=get_right_affix(res,preorsuf,0);
                if(preorsuf) {
                    copy_pref(res,fgt);
                } else {
                    copy_suff(res,fgt);
                }
                tmp = fgt->true_ft->d->deeper->true_ft->single->
                true_node->internal_node;
                int i;
                for(i=0;i<3;i++){
                    if (tmp[i] == NULL) break;
                    new_affix->size += tmp[i]->size;
                    new_affix->nodes[i]=tmp[i];
                    tmp[i]->ref_count++;
                }
                
                res->size = res->true_ft->d->prefix->size + res->true_ft->d->suffix->size + res->true_ft->d->deeper->size;
            }
            else{
                /* the deeper is of type deep*/
                node** getres;
                res = create_deep();
                if(preorsuf) {
                    copy_pref(res,fgt);
                } else {
                    copy_suff(res,fgt);
                }
                view tmp =ft_delete(fgt->true_ft->d->deeper,preorsuf);
                new_affix=get_right_affix(res,preorsuf,0);
                getres = (tmp.elem)->true_node->internal_node;
                for(int i=0;i<3;i++){
                    if (getres[i] == NULL) break;
                    
                    new_affix->size += getres[i]->size;
                    new_affix->nodes[i]=getres[i];
                    getres[i]->ref_count++;
                }
                res->true_ft->d->deeper= tmp.fg;
                
                res->size = res->true_ft->d->prefix->size + res->true_ft->d->suffix->size + res->true_ft->d->deeper->size;
            }
        }
    }
    stres.fg=res;
    
    // Postconditions & Invariants
    checkInvariants(stres.fg);
    
    return stres;
}

void copy_pref(ft*res,ft* fgt){
    int i,j;

    for(i=0,j=0;i<fgt->true_ft->d->prefix->size;i += fgt->true_ft->d->prefix->nodes[j]->size,j++){
        res->true_ft->d->prefix->nodes[j]=fgt->true_ft->d->prefix->nodes[j];
        fgt->true_ft->d->prefix->nodes[j]->ref_count++;
    }
    res->true_ft->d->prefix->size = fgt->true_ft->d->prefix->size;
}

void copy_suff(ft*res,ft* fgt){
    int i,j;
    for(i=0,j=0;i<fgt->true_ft->d->suffix->size;i += fgt->true_ft->d->suffix->nodes[j]->size,j++){
        res->true_ft->d->suffix->nodes[j]=fgt->true_ft->d->suffix->nodes[j];
        fgt->true_ft->d->suffix->nodes[j]->ref_count++;
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
    /* TODO
    checkInvariants(ft1);
    checkInvariants(ft2);
     */
    
    ft* res;
    list* l=NULL;
    res = concat_w_middle(ft1,l,ft2);
    
    // Postconditions & Invariants
    /* TODO
    checkInvariants(res);
     */
    return res;
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

        puts("before");
        list_display(l);
        l = nodes(l);
        puts("after");
        list_display(l);

        deeper_tmp = concat_w_middle(ft1->true_ft->d->deeper,l,
            ft2->true_ft->d->deeper);
        res->true_ft->d->deeper = deeper_tmp;
    }
    return res;
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

        tmp2 = create_node_node();
        tmp2->true_node->internal_node[0] = l->next->next->elem;
        tmp2->true_node->internal_node[1] = l->next->next->next->elem;

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
        l=removel(l);
        l=removel(l);
        l=removel(l);
        res = nodes(l);
        res = add(tmp,res);
    }
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
