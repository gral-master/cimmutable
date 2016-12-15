/*
 * main.c
 */

#include <stdio.h>

#include "fingertree.h"

int main(int argc, char **argv)
{
    ft* fingerTree = ft_init();
    int data = 4;

    ft_add((void*)&data, fingerTree);

    ft_display(fingerTree);

    return 0;
}

