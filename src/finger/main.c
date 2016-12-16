/*
 * main.c
 */

#include <stdio.h>

#include "fingertree.h"

int main(int argc, char **argv)
{
    ft* fingerTree = create_empty();
    int data = 4, data2 = 8, data3=10, data4=12;
    printf("2:\n");
    ft* fingerTree2 = ft_add((void*)&data, fingerTree);
    printf("3:\n");
    ft* fingerTree3 = ft_add((void*)&data2, fingerTree2);
    ft* fingerTree4=  ft_add((void*)&data3, fingerTree3);
     ft* fingerTree5=  ft_add((void*)&data4, fingerTree4);
    printf("Empty:\n");
    ft_display(fingerTree);

    printf("\nSingle:\n");
    ft_display(fingerTree2);

    printf("\nDeep\n");
    ft_display(fingerTree3);

    printf("\nDeep2\n");
    ft_display(fingerTree4);

    printf("\nDeep3\n");
    ft_display(fingerTree5);


    return 0;
}
