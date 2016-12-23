/*
 * main.c
 */

#include <stdio.h>

#include "fingertree.h"
#include "list.h"

int main(int argc, char **argv)
{
  /* view x,y,z; */
  /*   ft* fingerTree = create_empty(); */
  /*   int data = 4, data2 = 8, data3=10, data4=12, data5=15, data6 = 20, data7=25, data8=30 ,data9=35 ,data10=40; */
  /*   printf("2:\n"); */
  /*   ft* fingerTree2 = ft_add((void*)&data, fingerTree,0); */
  /*   printf("3:\n"); */
  /*   ft* fingerTree3=  ft_add((void*)&data3, fingerTree2,0); */
  /*   ft* fingerTree4=  ft_add((void*)&data4, fingerTree3,0); */
  /*   ft* fingerTree5=  ft_add((void*)&data5, fingerTree4,0); */
  /*   ft* fingerTree6=  ft_add((void*)&data6, fingerTree5,0); */
  /*   ft* fingerTree7=  ft_add((void*)&data7, fingerTree6,0); */
  /*   ft* fingerTree8=  ft_add((void*)&data8, fingerTree7,0); */
  /*   ft* fingerTree9=  ft_add((void*)&data9, fingerTree8,0); */
  /*   ft* fingerTree10=  ft_add((void*)&data10, fingerTree9,0); */
  ft* f[21];
  int tmp[20];
  f[0]=create_empty();
  int i;
  for(i=1;i<10;i++)
    {
      tmp[i]=i;
      f[i]=  ft_add((void*)&tmp[i], f[i-1],0);

    }
  f[10]=create_empty();
    for(i=11;i<20;i++)
    {
      tmp[i]=i;
      f[i]=  ft_add((void*)&tmp[i], f[i-1],1);

    }

     ft_display(f[9]);
     puts("///////////////");
      ft_display(f[19]);
      puts("/////////////////");
    f[20]=concat(f[9],f[19]);
   
    puts("**********************");
  ft_display(f[20]);
  /* for(i=10;i<20;i++) */
  /*   { */
  /*     f[i]=  ft_add((void*)&i, f[i-1],0); */

  /*   } */
  
    
  /*   x = ft_delete(fingerTree10,0); */
  /*   y = ft_delete(x.fg,0); */
  /*   z = ft_delete(y.fg,0); */
    /* x=ft_delete(fingerTree7,0); */
    /* ft*fingerTree8 = x.fg; */
    /*  ft* fingerTree9=  ft_add((void*)&data8, fingerTree8,1); */
    /*   ft* fingerTree10=  ft_add((void*)&data9, fingerTree9,1); */
    /*    ft* fingerTree11=  ft_add((void*)&data10, fingerTree10,1); */
    /* fflush(stdout); */
    
    /*  printf("Empty:\n"); */
    /*  ft_display(fingerTree); */

    /*  printf("\nSingle:\n"); */
    /*  ft_display(fingerTree2); */

    /* printf("\nDeep\n"); */
    /* ft_display(fingerTree3); */

    /* printf("\nDeep2\n"); */
    /* ft_display(fingerTree4); */
    
    /* printf("\nDeep3\n"); */
    /* ft_display(fingerTree5); */

    
    /* printf("\nDeep4\n"); */
    /* ft_display(fingerTree6); */

    /* printf("\nDeep5\n"); */
    /*   ft_display(fingerTree7); */
    /* printf("\nDeep8\n"); */
    /*  ft_display(fingerTree8); */

    /*  printf("\nDeep9\n"); */
    /*  ft_display(fingerTree9); */

     
    /*  printf("\nDeep10\n"); */
    /*  ft_display(fingerTree10); */
    /*  puts(""); */
    /* ft_display(x.fg); */
    /* puts(""); */
    /* ft_display(y.fg); */
    /* puts(""); */
    /* ft_display(z.fg); */
    /* printf("\nDeep7\n"); */
    /* ft_display(fingerTree9); */

    
    /* printf("\nDeep8\n"); */
    /* ft_display(fingerTree10); */

    
    /* printf("\nDeep9\n"); */
    /* ft_display(fingerTree11); */
  /* node* tab[7]; */
  /* list* l = NULL; */
  /* int i; */
  /* int t=5; */
  /* for(i=0;i<2;i++){ */
  /*   tab[i]=create_data_node(); */
  /*   tab[i]->true_node->data =(void*) &t; */
  /*   l=add(tab[i],l); */
  /* } */
  /* l=nodes(l); */
  /* /\* while(l!=NULL){ *\/ */
  /* /\*   printf("boucle\n"); *\/ */
  /* /\*   node_display(l->elem); *\/ */
  /* /\*   printf(","); *\/ */
  /* /\*   l=l->next; *\/ */
  /* /\* } *\/ */
  /* /\* puts(""); *\/ */

  
    return 0;
}
