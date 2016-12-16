/*
 * main.c
 */

#include <stdio.h>

#include "fingertree.h"

int main(int argc, char **argv)
{
    ft* fingerTree = create_empty();
    int data = 4, data2 = 8;
    printf("2:\n");
    ft* fingerTree2 = ft_add((void*)&data, fingerTree);
    printf("3:\n");
    ft* fingerTree3 = ft_add((void*)&data2, fingerTree2);

    printf("Empty:\n");
    ft_display(fingerTree);

    printf("\nSingle:\n");
    ft_display(fingerTree2);

    printf("\nDeep\n");
    ft_display(fingerTree3);


    return 0;
}
