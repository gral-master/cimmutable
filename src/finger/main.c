/*
* main.c
*/

#include <stdio.h>
#include <stdlib.h>

#include "fingertree.h"

#define MAX_ADD 10
#define MAX_DELETE 2

void procedure(){

 // ft_add
    int data[MAX_ADD];
    int data2[MAX_ADD];
    for (int i = 0; i < MAX_ADD; i++) {
        data[i] = i*2;
	data2[i] =i*2 +1; 
    }
    
    //simulation_add(200,5,800,1000);
    
    ft* fingerTreeArray[MAX_ADD+1];
    ft* fingerTreeArray2[MAX_ADD+1];
    
    fingerTreeArray[0] = create_empty();
    for (int i = 1; i < MAX_ADD+1; i++) {
      printf("add i:%d\n",i);
        fingerTreeArray[i] = ft_add((void*)&data[i-1],fingerTreeArray[i-1],0);
        ft_display(fingerTreeArray[i]);
        puts("\n****************************");
    }
    puts("////////////////////////////////////////////");

    fingerTreeArray2[0] = create_empty();
    for (int i = 1; i < MAX_ADD+1; i++) {
      printf("add2 i:%d\n",i);
        fingerTreeArray2[i] = ft_add((void*)&data2[i-1],fingerTreeArray2[i-1],1);
        ft_display(fingerTreeArray2[i]);
        puts("\n****************************");
    }

    /* ft* fingerTreeArray2[MAX_ADD+1]; */
    /* fingerTreeArray2[0] = create_empty(); */
    /* for (int i = 1; i < MAX_ADD+1; i++) { */
    /*   fingerTreeArray2[i] = ft_add((void*)&data[MAX_ADD+i],fingerTreeArray2[i-1],1); */
    /* 	 ft_display(fingerTreeArray2[i]); */
    /* 	 puts("\n****************************"); */
    /* } */
      /*   puts("\nconcat\n"); */
    puts("\nconcat");
    ft* res = ft_concat(fingerTreeArray[MAX_ADD],fingerTreeArray2[MAX_ADD]);
      ft_display(res);
      
      ft_unref(res);

     
     // ft_delete
    puts("");
    /* puts("delete/////////////"); */
    /*  view tmp; */
    /*  ft* fingerTreeArray1[MAX_ADD+1]; */
    /*  fingerTreeArray1[0]= fingerTreeArray[MAX_ADD]; */
    /*  ft_display(fingerTreeArray[MAX_ADD]); */
    /*  for(int i = 1; i< MAX_ADD+1;i++){ */
    /*   tmp= ft_delete(fingerTreeArray1[i-1],1); */
    /*   fingerTreeArray1[i] = tmp.fg; */
    /*   printf("\nelem: "); */
    /*   node_display(tmp.elem); */
    /*   printf("\ntree: "); */
    /*   ft_display(fingerTreeArray1[i]); */
    /*   printf("\n"); */
     
      //      free(tmp.elem);
      /*}*/
    
    /* ft_display(fingerTreeArray1[MAX_ADD]); */
    /* printf("\n"); */
    
    // ft_split
    /* int sindex = MAX_ADD*1/3; */
    /* split sres = ft_split(fingerTreeArray[MAX_ADD], sindex); */
    
    /* printf("Split: content of node at index %d: %d\n", sindex, *((int*)sres.node->true_node->data)); */
    /* printf("left finger tree for %d:\n", sindex); */
    /* ft_display(sres.ft1); */
    /* printf("\nright finger tree for %d:\n", sindex); */
    /* ft_display(sres.ft2); */
    /* printf("\n"); */
    
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
     puts("\nstart unref add");
     for (int i = 0; i < MAX_ADD+1; i++) {
       printf("\nboucle unref %d\n",i);
        ft_display(fingerTreeArray[i]);
	ft_display(fingerTreeArray2[i]);
        ft_unref(fingerTreeArray[i]);
	ft_unref(fingerTreeArray2[i]);
	
     }	
    
     /* puts("start unref delete"); */

     /*    for (int i = 1; i < MAX_ADD+1; i++)  */
     /*         	 ft_unref(fingerTreeArray1[i]); */
    

    
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


}


int main(int argc, char **argv)
{

  
   
    return 0;
}


