#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "./src/rrb_vector/rrb_vect.h"
//Compile with gcc main.c obj/*.o -lm

char* print(imc_data_t* data) {
  char *str = malloc(sizeof(char) * 15);
  sprintf(str, "%d", data->id);
  return str;
}

int next_rand(int a, int b) {
  return a + rand() % b;
}

int main() {
  /* Seed generation */
  time_t t;
  srand((unsigned) time(&t));

  /* rrb and data definition */
  imc_rrb_t* vec = imc_rrb_create();
  imc_data_t* data;

  /* push and print n times */
  puts("\n\nBEGIN PUSH AND PRINT TESTING\n\n");
  char *str = malloc(sizeof(char) * 100);
  for(int i = 0; i<33; i++) {
    sprintf(str, "rrb_size_%d.dot", i);
    imc_rrb_emit(vec, str, print);
    data = malloc(sizeof(imc_data_t));
    data->id = next_rand(0, 10);
    vec = imc_rrb_push(vec, data);
  }
  free(str);

  /* push n times */
  puts("\n\nBEGIN PUSH TESTING\n\n");
  for(int i = 0; i< 1050; i++) {
    data = malloc(sizeof(imc_data_t));
    data->id = next_rand(0, 10);
    vec = imc_rrb_push(vec, data);
  }

  /* lookup n random indexes */
  puts("\n\nBEGIN LOOKUP TESTING\n\n");
  for(int i = 0; i< 10; i++) {
    int n = next_rand(0, imc_rrb_size(vec));
    data = imc_rrb_lookup(vec, n);
    printf("%dth data : %d\n", n, data->id);
  }

  /* update and lookup n random indexes */
  puts("\n\nBEGIN UPDATE TESTING\n\n");
  for(int i = 0; i< 10; i++) {
    int n = next_rand(0, imc_rrb_size(vec));
    data = imc_rrb_lookup(vec, n);
    printf("%dth data : %d\n", n, data->id);
    data = malloc(sizeof(imc_data_t));
    data->id = next_rand(0, 10);
    vec = imc_rrb_update(vec, n, data);
    data = imc_rrb_lookup(vec, n);
    printf("%dth data : %d\n", n, data->id);
  }

  /* pop n indexes */
  puts("\n\nBEGIN POP TESTING\n\n");
  for(int i = 0; i< 10; i++) {
    data = imc_rrb_lookup(vec, imc_rrb_size(vec)-1);
    printf("Last data : %d\n", data->id);
    vec = imc_rrb_pop(vec, &data);
    printf("Popped data : %d\n", data->id);
    free(data);
  }

  /* print, split and print again */
  puts("\n\nBEGIN SPLIT TESTING\n\n");
  sprintf(str, "split_before.dot");
  imc_rrb_emit(vec, str, print);

  imc_rrb_t* vec_left;
  imc_rrb_t* vec_right;
  imc_rrb_split(vec, imc_rrb_size(vec)/2, &vec_left, &vec_right);

  sprintf(str, "split_left.dot");
  imc_rrb_emit(vec_left, str, print);

  sprintf(str, "split_right.dot");
  imc_rrb_emit(vec_right, str, print);


}
