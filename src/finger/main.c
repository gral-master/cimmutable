/*
* main.c
*/

#include <stdio.h>

#include "fingertree.h"

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
    /*
    view x,y,z;
    ft* fingerTree = create_empty();
    int data1 = 0, data3=5, data4=10, data5=15, data6 = 20;
    int data7=25, data8=30 ,data9=35 ,data10=40;
    int data11 = 45, data12 = 50, data13=55, data14=60, data15=65, data16 = 70;
    int data17=75, data18=80 ,data19=85;
    ft* fingerTree2 = ft_add((void*)&data1, fingerTree,0);
    ft* fingerTree3=  ft_add((void*)&data3, fingerTree2,0);
    ft* fingerTree4=  ft_add((void*)&data4, fingerTree3,0);
    ft* fingerTree5=  ft_add((void*)&data5, fingerTree4,0);
    ft* fingerTree6=  ft_add((void*)&data6, fingerTree5,0);
    ft* fingerTree7=  ft_add((void*)&data7, fingerTree6,0);
    ft* fingerTree8=  ft_add((void*)&data8, fingerTree7,0);
    ft* fingerTree9=  ft_add((void*)&data9, fingerTree8,0);
    ft* fingerTree10=  ft_add((void*)&data10, fingerTree9,0);
    ft* fingerTree11=  ft_add((void*)&data11, fingerTree10,0);
    ft* fingerTree12 = ft_add((void*)&data12, fingerTree11,0);
    ft* fingerTree13=  ft_add((void*)&data13, fingerTree12,0);
    ft* fingerTree14=  ft_add((void*)&data14, fingerTree13,0);
    ft* fingerTree15=  ft_add((void*)&data15, fingerTree14,0);
    ft* fingerTree16=  ft_add((void*)&data16, fingerTree15,0);
    ft* fingerTree17=  ft_add((void*)&data17, fingerTree16,0);
    ft* fingerTree18=  ft_add((void*)&data18, fingerTree17,0);
    ft* fingerTree19=  ft_add((void*)&data19, fingerTree18,0);*/
    /*
    x = ft_delete(fingerTree10,0);
    y = ft_delete(x.fg,0);
    z = ft_delete(y.fg,0);
    */
    /* x=ft_delete(fingerTree7,0); */
    /* ft*fingerTree8 = x.fg; */
    /*  ft* fingerTree9=  ft_add((void*)&data8, fingerTree8,1); */
    /*   ft* fingerTree10=  ft_add((void*)&data9, fingerTree9,1); */
    /*    ft* fingerTree11=  ft_add((void*)&data10, fingerTree10,1); */
    /* fflush(stdout); */
/*
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
    printf("\nDeep19\n");
    ft_display(fingerTree19);
    */
    /*
    puts("");
    ft_display(x.fg);
    puts("");
    ft_display(y.fg);
    puts("");
    ft_display(z.fg);
    */
    /* printf("\nDeep7\n"); */
    /* ft_display(fingerTree9); */


    /* printf("\nDeep8\n"); */
    /* ft_display(fingerTree10); */


    /* printf("\nDeep9\n"); */
    /* ft_display(fingerTree11); */


    return 0;
}
