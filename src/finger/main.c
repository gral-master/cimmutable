/*
 * main.c
 */

#include <stdio.h>

#include "fingertree.h"

int main(int argc, char **argv)
{
    ft* fingerTree = ft_init();
    int data = 4;

    ft* fingerTree2 = ft_add((void*)&data, fingerTree);

    printf("Empty:\n");
    ft_display(fingerTree);

    printf("Single:\n");
    ft_display(fingerTree2);

    return 0;
}
