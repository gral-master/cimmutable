/*
 * main.c
 */

#include <stdio.h>

#include "fingertree.h"

int main(int argc, char **argv)
{
  view x,y,z;
    ft* fingerTree = create_empty();
    int data = 4, data2 = 8, data3=10, data4=12, data5=15, data6 = 20, data7=25, data8=30 ,data9=35 ,data10=40;
    printf("2:\n");
    ft* fingerTree2 = ft_add((void*)&data, fingerTree,0);
    printf("3:\n");
    ft* fingerTree3=  ft_add((void*)&data3, fingerTree2,0);
    ft* fingerTree4=  ft_add((void*)&data4, fingerTree3,0);
    ft* fingerTree5=  ft_add((void*)&data5, fingerTree4,0);
    ft* fingerTree6=  ft_add((void*)&data6, fingerTree5,0);
    ft* fingerTree7=  ft_add((void*)&data7, fingerTree6,0);
    ft* fingerTree8=  ft_add((void*)&data8, fingerTree7,0);
    ft* fingerTree9=  ft_add((void*)&data9, fingerTree8,0);
    ft* fingerTree10=  ft_add((void*)&data10, fingerTree9,0);
    
    
    x = ft_delete(fingerTree10,0);
    y = ft_delete(x.fg,0);
    z = ft_delete(y.fg,0);
    /* x=ft_delete(fingerTree7,0); */
    /* ft*fingerTree8 = x.fg; */
    /*  ft* fingerTree9=  ft_add((void*)&data8, fingerTree8,1); */
    /*   ft* fingerTree10=  ft_add((void*)&data9, fingerTree9,1); */
    /*    ft* fingerTree11=  ft_add((void*)&data10, fingerTree10,1); */
    /* fflush(stdout); */
    
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

    
    printf("\nDeep4\n");
    ft_display(fingerTree6);

    printf("\nDeep5\n");
      ft_display(fingerTree7);
    printf("\nDeep8\n");
     ft_display(fingerTree8);

     printf("\nDeep9\n");
     ft_display(fingerTree9);

     
     printf("\nDeep10\n");
     ft_display(fingerTree10);
     puts("");
    ft_display(x.fg);
    puts("");
    ft_display(y.fg);
    puts("");
    ft_display(z.fg);
    /* printf("\nDeep7\n"); */
    /* ft_display(fingerTree9); */

    
    /* printf("\nDeep8\n"); */
    /* ft_display(fingerTree10); */

    
    /* printf("\nDeep9\n"); */
    /* ft_display(fingerTree11); */


    return 0;
}
