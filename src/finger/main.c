/*
* main.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "fingertree.h"
#include "imc_ft_vector.h"

#define MAX_ADD 10
#define MAX_DELETE 2

void ft_test() {
    // === ft_add ===
    int data[2*MAX_ADD];
    for (int i = 0; i < 2*MAX_ADD; i++) {
        data[i] = i * 5;
    }
    
    //simulation_add(200,5,800,1000);
    
    ft* fingerTreeArray[MAX_ADD+1];
    
    fingerTreeArray[0] = create_empty();
    for (int i = 1; i < MAX_ADD+1; i++) {
        fingerTreeArray[i] = ft_add((void*)&data[i-1],fingerTreeArray[i-1],1);
        //ft_display(fingerTreeArray[i]);
        //puts("\n****************************");
    }
    
    // === ft_concat ===
    /* ft* fingerTreeArray2[MAX_ADD+1]; */
    /* fingerTreeArray2[0] = create_empty(); */
    /* for (int i = 1; i < MAX_ADD+1; i++) { */
    /*   fingerTreeArray2[i] = ft_add((void*)&data[MAX_ADD+i],fingerTreeArray2[i-1],1); */
    /* 	 ft_display(fingerTreeArray2[i]); */
    /* 	 puts("\n****************************"); */
    /* } */
    /* puts("concat"); */
    /* ft_display(fingerTreeArray[MAX_ADD-1]); */
    /* puts("2"); */
    /*    ft_display(fingerTreeArray[MAX_ADD]); */
    /* puts("concat"); */
    /* ft* res = ft_concat(fingerTreeArray[MAX_ADD-1],fingerTreeArray[MAX_ADD]); */
    /*   ft_display(res); */
    
    // === ft_delete ===
    /*
     puts("");
     puts("delete/////////////");
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
     
     //      free(tmp.elem);
     }
     */
     
    // === ft_delete (with loop) ===
    /* view viewArray[MAX_DELETE+1]; */
    /* viewArray[0].fg = fingerTreeArray[MAX_ADD]; */
    
    /* for (int i = 1; i < MAX_DELETE+1; i++) { */
    /*     viewArray[i] = ft_delete(viewArray[i-1].fg, 1); */
    /*     printf("removed: %d rest:\n",*((int*)viewArray[i].elem->true_node->data)); */
    /*     ft_display(viewArray[i].fg); */
    /*     printf("\n"); */
    /* } */
    printf("===============================\n");
    ft_display(fingerTreeArray[MAX_ADD]);
    printf("\n=============================\n");
    
    // === ft_split ===
    int sindex = MAX_ADD*1/3;
    
    split sres = ft_split(fingerTreeArray[MAX_ADD], sindex);
    
    printf("Split:\n");
    printf("===left finger tree for %d:===\n", sindex);
    ft_display(sres.ft1);
    printf("\n===right finger tree for %d:===\n", sindex);
    ft_display(sres.ft2);
    printf("\n===node for %d:===\n", sindex);
    node_display(sres.node);
    printf("\n=======================\n");
    puts("start unref sres.ft1");
    ft_unref(sres.ft1);
    puts("start unref sres.ft2");
    ft_unref(sres.ft2);
    puts("start unref sres.node");
    node_unref(sres.node);
    
    
    // === ft_lookup ===
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
    
    // === ft_unref ===
    puts("start unref add");
    for (int i = 0; i < MAX_ADD+1; i++)
        ft_unref(fingerTreeArray[i]);
  
    puts("end unref");
}

void vector_test() {
    int data[MAX_ADD], data2[MAX_ADD], data_u = 127, *data_p = NULL;
    for (int i = 0; i < MAX_ADD; i++) {
        data[i] = i * 5;
        data2[i] = i * 3;
    }
    
    imc_vector_t* vectorArray[MAX_ADD+1];
    imc_vector_t* vectorArray2[MAX_ADD+1];;
    
    // create & push
    vectorArray[0] = imc_vector_create();
    vectorArray2[0] = imc_vector_create();
    for (int i = 1; i < MAX_ADD+1; i++) {
        vectorArray[i] = imc_vector_push(vectorArray[i-1], ((void*)&data[i-1]));
        vectorArray2[i] = imc_vector_push(vectorArray2[i-1], ((void*)&data2[i-1]));
    }
    imc_vector_t* max_vector = vectorArray[MAX_ADD];
    imc_vector_t* max_vector2 = vectorArray2[MAX_ADD];
    
    // size
    assert(imc_vector_size(max_vector) == MAX_ADD);
    
    // update
    imc_vector_t* v_updated = imc_vector_update(max_vector, MAX_ADD/2, &data_u);
    printf("\n===updated===\n");
    imc_vector_dump(v_updated);
    
    // lookup
    assert(*imc_vector_lookup(v_updated, MAX_ADD/2) == data_u);
    
    // pop
    imc_vector_t* v_popped = imc_vector_pop(max_vector, &data_p);
    assert(imc_vector_size(v_popped) == MAX_ADD-1);
    assert(*data_p == (MAX_ADD-1)*5);
    
    // split
    imc_vector_t *vec_out1 = NULL, *vec_out2 = NULL;
    imc_vector_split(max_vector, MAX_ADD/2, &vec_out1, &vec_out2);
    
    // dump
    printf("===vec_out1===\n");
    imc_vector_dump(vec_out1);
    printf("\n===vec_out2===\n");
    imc_vector_dump(vec_out2);
    
    // merge
    imc_vector_t *merged = imc_vector_merge(max_vector, max_vector2);
    printf("\n===max_vector===\n");
    imc_vector_dump(max_vector);
    printf("\n===max_vector2===\n");
    imc_vector_dump(max_vector2);
    printf("\n===merged===\n");
    imc_vector_dump(merged);
    printf("\n\n");
    
    // unref
    for (int i = 0; i < MAX_ADD+1; i++) {
        imc_vector_unref(vectorArray[i]);
        imc_vector_unref(vectorArray2[i]);
    }
    imc_vector_unref(v_updated);
    imc_vector_unref(v_popped);
    imc_vector_unref(vec_out1);
    imc_vector_unref(vec_out2);
    imc_vector_unref(merged);
}

int main(int argc, char **argv)
{
    ft_test();
    vector_test();
    
    return 0;
}
