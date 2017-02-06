#include <stdio.h>
#include <stdlib.h>
#include "list.h"


list* create_lempty(){
  list*l = NULL;
  return l;
}


list* create_lelem(){
  list* res = malloc(sizeof(list));
  res->elem=NULL;
  res->next=NULL;
  return res;
}

list* add(node* x, list* l){
   if(x==NULL)
    return l;
  list* res = create_lelem();
  res->elem = x;
  res->next=l;
  return res;

}

list* removel(list* l){
   list* iter = l;
  if(l==NULL)
    exit(-1);
  iter=l->next;
  free(l);
  return iter;

}

list* remove_last(list* l){
   list* iter =l;
  if(iter==NULL)
    exit(-1);
  if(iter->next==NULL){
    free(iter);
    return NULL;
  }
  while(iter->next->next!=NULL){
    iter=iter->next;
  }
  free(iter->next);
  iter->next=NULL;
  return l;
}

node* first(list* l){
   if(l==NULL)
     exit(-1);
  return l->elem;

}

list* concat(list* l1, list* l2){
  list* iter=l1;
  if(iter==NULL)
    return l2;
  while(iter->next!=NULL)
    iter= iter->next;
  iter->next = l2;
  return l1;
}

node* last(list* l){
  node* res;
  list* iter = l;
  while(iter->next!=NULL)
    iter= iter->next;
  res = iter->elem;
  return res;
}

void list_display(list*l){
  if(l==NULL){
    printf("liste_vide\n");
    return;
  }
  list*tmp=l;
  printf("list : ");
  node_display(l->elem);
  printf(",");
  tmp=tmp->next;
  list_display(tmp);
  
}
