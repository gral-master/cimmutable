#include <stdio.h>
#include <stdlib.h>
#include "fingertree.h"
#include "list.h"
ft* create_empty() {
    ft* fgt = malloc(sizeof(ft));
    fgt->type = EMPTY_TYPE;
    fgt->ref_count = 1;
    fgt->size = 0;
    return fgt;
}

ft* create_single(node* n){
    ft* fgt = malloc(sizeof(ft));
    fgt->type = SINGLE_TYPE;
    fgt->true_ft = malloc(sizeof(true_ft_t));
    fgt->true_ft->single = n;
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

node* create_data_node(void* data){
    node* n = malloc(sizeof(node));
    n->type = DATA_TYPE;
    n->true_node = malloc(sizeof(true_node_t));
    n->true_node->data = data;
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
    node* new_elem = create_data_node(data);
    res=add_elem_deep_recur(fgt,preorsuf,new_elem);
    res->size = fgt->size+1;
    return res;
}

/*
* For the moment, only works for trees with 1 element */
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
            if (fgt->true_ft->d->prefix[i] != NULL){
                node_display(fgt->true_ft->d->prefix[i]);
                printf(",");
            }
        }
        printf("],");
        // Deeper
        ft_display(fgt->true_ft->d->deeper);
        // Suffix
        printf(",[");
        for (int i = 3; i >=0; i--){
            if (fgt->true_ft->d->suffix[i] != NULL){
                node_display(fgt->true_ft->d->suffix[i]);
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
* this functions is recursive it buils the resulting finger tree.. the recurisvity occurs
* when there is no more space left in the prefix or suffix
*/
ft* add_elem_deep_recur(ft* fgt,int preorsuf,node*data_node){
    ft*res;
    node**src;
    node**dst;
    int i;
    if(fgt->type==EMPTY_TYPE){
        res = create_single(data_node);
    }
    else if(fgt->type == SINGLE_TYPE) {
        res = create_deep();
        fgt->true_ft->single->ref_count++;
        if(preorsuf){
            src=res->true_ft->d->suffix;
            dst=res->true_ft->d->prefix;
        }
        else{
            dst=res->true_ft->d->suffix;
            src=res->true_ft->d->prefix;
        }
        dst[0] = fgt->true_ft->single;
        src[0]=data_node;
    }

    else{
        res = create_deep();
        int index=check_available_space(fgt,preorsuf);
        dst=get_right_affix(res,preorsuf,0);
        src=get_right_affix(fgt,preorsuf,0);

        /* copy the pref and suff*/
        copy_pref(res,fgt);
        copy_suff(res,fgt);
        if(index==-1) {
            node* new_node = create_node_node();
            int i;
            for(i=0;i<3;i++){
                new_node->true_node->internal_node[i] = src[i+1];
                src[i+1]->ref_count++;
            }
            /* creating the new suffix or prefix*/
            dst[0]=data_node;
            dst[1]=src[0];
            src[0]->ref_count++;
            dst[2]=NULL;dst[3]=NULL;
            /* recursive call to the same function with deeper*/
            res->true_ft->d->deeper=add_elem_deep_recur(fgt->true_ft->d->deeper,preorsuf,new_node);
        }
        else{
            /* we can add directly, there is space left in prefix or suffix*/
            res->true_ft->d->deeper = fgt->true_ft->d->deeper;
            fgt->true_ft->d->deeper->ref_count++;
            for(i=index-1;i>=0;i--)
            dst[i+1]=dst[i];

            /* add the new element*/
            dst[0]=data_node;
        }
    }

    return res;
}

/* checks if there is available space in the suffix of the deep
* to make it generic we use
* preorsuf to search in the prefix or suffix (0 for prefix, 1 for suffix)
*/
int check_available_space(ft* fgt,int preorsuf){
    if(fgt->type!=DEEP_TYPE){
        printf("error not a deep\n");
        exit(-1);
    }
    node**src;
    if(preorsuf){
        src = fgt->true_ft->d->suffix;
    }
    else{
        src = fgt->true_ft->d->prefix;
    }

    int i,res=-1;
    for(i=0;i<4;i++){
        if(src[i]==NULL){
            res=i;
            break;
        }
    }
    return res;
}


view ft_delete(ft* fgt,int preorsuf){
    ft* res;
    view stres;
    node**src;
    node**dst;
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

        if(preorsuf) stres.elem = fgt->true_ft->d->suffix[0];
        else stres.elem = fgt->true_ft->d->prefix[0];


        int index = check_available_space(fgt,preorsuf);
        if(index==-1 || index>1){
            int i;
            /* the tree is of type deep*/
            res = create_deep();
            dst=get_right_affix(res,preorsuf,0);
            src=get_right_affix(fgt,preorsuf,0);
            if(index==-1)index=3;
            /* here is the simple case*/
            copy_pref(res,fgt);
            copy_suff(res,fgt);
            res->true_ft->d->deeper = fgt->true_ft->d->deeper;
            fgt->true_ft->d->deeper->ref_count++;
            for(i=0;i<index;i++)
            dst[i]=dst[i+1];
            dst[index-1]=NULL;
        }
        else{
            /* there is only one element in the affix*/
            if(fgt->true_ft->d->deeper->type==EMPTY_TYPE){
                /* the deeper is empty, we look the other side and see if we can take one element*/
                int inv;
                if(preorsuf)inv=0;else inv=1;
                index = check_available_space(fgt,inv);
                if(index!=1){
                    if(index==-1)index=3;
                    else index--;
                    /* we can take an element from the other side*/
                    res=create_deep();
                    dst=get_right_affix(res,preorsuf,0);
                    src=get_right_affix(fgt,preorsuf,1);
                    dst[0]=src[index];
                    src[index]->ref_count++;
                    if(preorsuf)copy_pref(res,fgt); else copy_suff(res,fgt);
                    dst=get_right_affix(res,preorsuf,1);
                    dst[index]=NULL;
                }
                else{
                    src=get_right_affix(fgt,preorsuf,1);
                    res=create_single(src[0]);
                    src[0]->ref_count++;
                }
            }
            else if(fgt->true_ft->d->deeper->type==SINGLE_TYPE){
                node**tmp;
                res = create_deep();
                res->true_ft->d->deeper=create_empty();
                dst=get_right_affix(res,preorsuf,0);
                src=get_right_affix(fgt,preorsuf,1);
                if(preorsuf) copy_pref(res,fgt); else copy_suff(res,fgt);
                tmp = fgt->true_ft->d->deeper->true_ft->single->true_node->internal_node;
                int i;
                for(i=0;i<3;i++){
                    dst[i]=tmp[i];
                    tmp[i]->ref_count++;
                }

            }
            else{
                /* the deeper is of type deep*/
                node* getres;
                res = create_deep();
                if(preorsuf) copy_pref(res,fgt); else copy_suff(res,fgt);
                view tmp =ft_delete(fgt->true_ft->d->deeper,preorsuf);
                dst=get_right_affix(res,preorsuf,0);
                src=get_right_affix(fgt,preorsuf,0);
                getres = tmp.elem;
                dst[0]=getres->true_node->internal_node[0];
                dst[1]=getres->true_node->internal_node[1];
                dst[2]=getres->true_node->internal_node[2];
                res->true_ft->d->deeper= tmp.fg;

            }
        }
    }
    stres.fg=res;
    return stres;
}



void copy_pref(ft*res,ft* fgt){
    int i;
    for(i=0;i<4;i++){
        res->true_ft->d->prefix[i]=fgt->true_ft->d->prefix[i];
        if(fgt->true_ft->d->prefix[i]!=NULL)fgt->true_ft->d->prefix[i]->ref_count++;
    }


}


void copy_suff(ft*res,ft* fgt){
    int i;
    for(i=0;i<4;i++){
        res->true_ft->d->suffix[i]=fgt->true_ft->d->suffix[i];
        if(fgt->true_ft->d->suffix[i]!=NULL)fgt->true_ft->d->suffix[i]->ref_count++;
    }


}


node ** get_right_affix(ft*res,int preorsuf, int inv){
    node**dst;
    if(inv){
        if(preorsuf)
        preorsuf=0;
        else
        preorsuf=1;
    }

    if(preorsuf)
    dst=res->true_ft->d->suffix;
    else
    dst=res->true_ft->d->prefix;
    return dst;
}


ft* ft_concat(ft* ft1,ft* ft2){
    ft* res;
    list* l=NULL;
    res = concat_w_middle(ft1,l,ft2);
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
    }
    else{
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

        deeper_tmp = concat_w_middle(ft1->true_ft->d->deeper,l,ft2->true_ft->d->deeper);
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
    }
    else{
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
    node** dst;
    list*l=NULL;
    if(preorsuf) dst=fg->true_ft->d->suffix; else dst=fg->true_ft->d->prefix;
    int i;
    for(i=0;i<4;i++){
        l=add(dst[i],l);
    }
    return l;

}


void checkInvariants() {
    // === Node Invariants===
    // Nodes are either NODE_TYPE or DATA_TYPE

    // A DATA_TYPE Node does not contain another Node

    // A NODE_TYPE Node does contain 2 or 3 Nodes

    // ===Finger tree Invariant===
    // Finger trees are either DEEP_TYPE, SINGLE_TYPE or EMPTY_TYPE

    // A SINGLE_TYPE Finger tree contains only one Node (and no affix).

    // Any Node in the affix and any Single at level 0 (the highest level) are
    // DATA_TYPE.

    // Any Node in the affix and any Single at level n (the deeper level, the
    // greater n) are NODE_TYPE^n(DATA_TYPE).

    // A DEEP_TYPE Finger tree contains affixes and a deeper Finger tree.

    // The affixes contain at least one elem each (i.e. at least 1 Node contains
    // effective data)

    // The affixes are arrays of size four (i.e. contains 4 Node, even if
    // unused)

    // In an affix containing n elements, these elements correspond to the n
    // leftmost indexes of the array.

    // ===ref_count Invariants===
    // TODO

    // ===size Invariants===
    // TODO

}
