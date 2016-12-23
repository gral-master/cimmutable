/*
* main.c
*/

#include <stdio.h>

#include "fingertree.h"
#include "list.h"

#define MAX_TEST 20

int main(int argc, char **argv)
{
    int data[MAX_TEST];
    for (int i = 0; i < MAX_TEST; i++) {
        data[i] = i * 5;
    }

    ft* fingerTreeArray[MAX_TEST+1];
    fingerTreeArray[0] = create_empty();
    for (int i = 1; i < MAX_TEST+1; i++) {
        fingerTreeArray[i] = ft_add((void*)&data[i-1],fingerTreeArray[i-1],0);
    }
    ft_display(fingerTreeArray[MAX_TEST]);
    printf("\n");
  
    return 0;
}
