/*
* main.c
*/

#include <stdio.h>

#include "fingertree.h"

#define MAX_ADD 50
#define MAX_DELETE 5

int main(int argc, char **argv)
{

  
    // ft_add
    int data[2*MAX_ADD];
    for (int i = 0; i < 2*MAX_ADD; i++) {
        data[i] = i * 5;
    }

    ft* x =  rand_gene(MAX_ADD,data);
    ft_display(x);
    
    ft* fingerTreeArray[MAX_ADD+1];
    
    fingerTreeArray[0] = create_empty();
    for (int i = 1; i < MAX_ADD+1; i++) {
        fingerTreeArray[i] = ft_add((void*)&data[i-1],fingerTreeArray[i-1],1);
        //ft_display(fingerTreeArray[i]);
        //puts("\n****************************");
    }

    /* ft* fingerTreeArray2[MAX_ADD+1]; */
    /* fingerTreeArray2[0] = create_empty(); */
    /* for (int i = 1; i < MAX_ADD+1; i++) { */
    /*   fingerTreeArray2[i] = ft_add((void*)&data[MAX_ADD+i],fingerTreeArray2[i-1],1); */
    /* 	 ft_display(fingerTreeArray2[i]); */
    /* 	 puts("\n****************************"); */
    /* } */

    /* puts("concat"); */
    /* ft* res = ft_concat(fingerTreeArray[MAX_ADD],fingerTreeArray2[MAX_ADD]); */
    /*   ft_display(res); */
    
    /*
     // ft_delete
    view tmp;
     ft* fingerTreeArray1[MAX_ADD+1];
     fingerTreeArray1[0]= fingerTreeArray[MAX_ADD];
     ft_display(fingerTreeArray[MAX_ADD]);
     for(int i = 1; i< MAX_ADD+1;i++){
      tmp= ft_delete(fingerTreeArray1[i-1],1);
      fingerTreeArray1[i] = tmp.fg;
      printf("\nelem: ");
      node_display(tmp.elem);
      printf("\ntree: ");
      ft_display(fingerTreeArray1[i]);
      printf("\n");
     }
     */
    
    ft_display(fingerTreeArray[MAX_ADD]);
    printf("\n");
    
    // ft_split
    int sindex = MAX_ADD*1/3;
    split sres = ft_split(fingerTreeArray[MAX_ADD], sindex);
    
    printf("Split: content of node at index %d: %d\n", sindex, *((int*)sres.node->true_node->data));
    printf("left finger tree for %d:\n", sindex);
    ft_display(sres.ft1);
    printf("\nright finger tree for %d:\n", sindex);
    ft_display(sres.ft2);
    printf("\n");
    
    // ft_lookup
    /*
    int index = MAX_ADD/2;
    int* lookup_res = (int*)ft_lookup(fingerTreeArray[MAX_ADD], index);
    printf("Lookup result for %d: %d\n", index, *lookup_res);
    index = MAX_ADD/4;
    lookup_res = (int*)ft_lookup(fingerTreeArray[MAX_ADD], index);
    printf("Lookup result for %d: %d\n", index, *lookup_res);
    index = MAX_ADD*3/4;
    lookup_res = (int*)ft_lookup(fingerTreeArray[MAX_ADD], index);
    printf("Lookup result for %d: %d\n", index, *lookup_res);
    index = MAX_ADD*1/3;
    lookup_res = (int*)ft_lookup(fingerTreeArray[MAX_ADD], index);
    printf("Lookup result for %d: %d\n", index, *lookup_res);
     */
    /* // ft_delete */
    /* view viewArray[MAX_DELETE+1]; */
    /* viewArray[0].fg = fingerTreeArray[MAX_ADD]; */
    
    /* for (int i = 1; i < MAX_DELETE+1; i++) { */
    /*     viewArray[i] = ft_delete(viewArray[i-1].fg, 1); */
    /*     printf("removed: %d rest:\n",*((int*)viewArray[i].elem->true_node->data)); */
    /*     ft_display(viewArray[i].fg); */
    /*     printf("\n"); */
    /* } */
    
    // ft_unref
    /*
    for (int i = 0; i < MAX_ADD+1; i++) {
        printf("%d\n", i);
        ft_unref(fingerTreeArray[i]);
    }
    */
    
    // ft_concat
   //   TODO
    /* puts("////////////////////////////////"); */
    /* ft_display(fingerTreeArray[MAX_ADD]); */
    /* puts("////////////////////////////////"); */
    /* ft_display(fingerTreeArray[MAX_ADD-1]); */
    /* puts("************************************"); */
    /* ft* concat_res = ft_concat(fingerTreeArray[MAX_ADD], fingerTreeArray[MAX_ADD-1]); */
    /* ft_display(concat_res); */
    /* printf("\n");     */
    return 0;
}
