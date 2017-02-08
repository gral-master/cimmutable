#ifndef LIST_H
#define LIST_H



#include "fingertree.h"


list* create_lempty();
list* create_lelem();
list* add(node* x,list* l);
list* removel(list* l);
list* concat(list* l1, list* l2);
node* first(list* l);
node* last(list* l);
void list_display(list*l);
list* remove_last(list* l);
#endif
