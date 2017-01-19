/************************************************************************
* Copyright © 2016 Matthieu Dien <matthieu.dien@lip6.fr>                *
* This work is free. You can redistribute it and/or modify it under the *
* terms of the Do What The Fuck You Want To Public License, Version 2,  *
* as published by Sam Hocevar. See the COPYING file for more details.   *
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "avl.h"

struct _avl_data_t {
  int content;
};
char* avl_data_as_string (avl_data_t* data) {
  if (data) {
    char* buf = malloc(15 * sizeof(char));
    sprintf(buf, "%d",data->content);
    return buf;
  } else {
    return NULL;
  }
}
avl_data_t* make_data(int i) {
  avl_data_t* data = malloc(sizeof *data);
  data->content = i;
  return data;
}

int compare(avl_data_t* d1, avl_data_t* d2) {
  if (d1->content == d2->content) return 0;
  else if (d1->content < d2->content) return -1;
  else return 1;
}

int main(int argc, char* argv[])
{
  int n, nbr = 0, nbs = 0;
  clock_t start_time;
  float total_time;

  /* initialize random seed: */
  srand(time(NULL));

  if (argc > 1) n = atoi(argv[1]);
  else n = 10000;

  avl_tree** t = malloc(sizeof(avl_tree*) * n+1);

  t[0] = avl_make_empty_tree(compare);

  start_time = clock();

  avl_tree* v = avl_make_empty_tree(compare);
  for (int i = 0; i < n; i++)  {
    avl_tree* tmp = avl_insert(v, make_data(i));
    avl_erase_tree(v);
    *v = *tmp;
  }
    avl_print(v);
  /* for (int i = 0; i < n; i++)  { */
  /*   t[i+1] = avl_insert(t[i], make_data(i)); */
  /* } */
  total_time = (clock() - start_time)/((float)CLOCKS_PER_SEC);
  /* for(int i=0; i<n; i++){ */
  /*   avl_data_t* tmp = NULL; */
  /*   if (i % 2 == 0) { */
  /*     t[n] = avl_remove(t[n] , make_data(i), &tmp); */
  /*     printf("Should have remove %d. Removed %d\n", i, tmp->content); */
  /*   } */
  /* } */
  /* avl_print(t[n-1]); */
  /* avl_print(t[n]); */
  
  printf("I did:\n- %d insertions\n- %d deletions\n- %d research\n in %f seconds\n", n, nbr, nbs, total_time);

  return 0;
}
