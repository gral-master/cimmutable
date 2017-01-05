#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "./src/rrb_vector/rrb_vect.h"
//Compile with gcc main.c obj/*.o -lm

char* print(imc_data_t* data) {
  char *str = malloc(sizeof(char) * 15);
  sprintf(str, "%d", data->id);
  return str;
}
int main() {
  imc_rrb_t* vec = imc_vector_concrete_create();

  imc_data_t* data = malloc(sizeof(int));
  data->id = 5;
  imc_rrb_t* vec2 = imc_vector_concrete_push(vec, data);

  imc_data_t* data2 = malloc(sizeof(int));
  data2->id = 7;
  imc_rrb_t* vec3 = imc_vector_concrete_push(vec2, data2);

  // imc_data_t* data_lookup = imc_vector_concrete_lookup(vec3, 0);
  // printf("found at 0 : %d \n", data_lookup->id);
  //
  // imc_data_t* data_lookup2 = imc_vector_concrete_lookup(vec3, 1);
  // printf("found at 1 : %d \n", data_lookup2->id);

imc_rrb_t* vec4;
  for(int i = 0; i< 1050; i++) {
    vec4 = imc_vector_concrete_push(vec3, data2);
    vec3 = imc_vector_concrete_push(vec4, data2);
  }
/*
  imc_data_t* popped = NULL;
  vec4 = imc_vector_concrete_pop(vec3, &popped);
  printf("last inserted : %d \n", popped->id);


  for(int i = 0; i< 1035; i++) {
    vec4 = imc_vector_concrete_pop(vec4, &popped);
    printf("last inserted : %d \n", popped->id);
  }

  for(int i = 0; i< 10305; i++) {
    vec4 = imc_vector_concrete_push(vec4, data);
  }
//lookup 10302
imc_data_t* data_lookup = imc_vector_concrete_lookup(vec4, 10302);
printf("found at 10302 : %d \n", data_lookup->id);
//update 10302
imc_data_t* newdata = malloc(sizeof(int));
newdata->id = 166;
vec4 = imc_vector_concrete_update(vec4, 10302, newdata);
//lookup 10302
data_lookup = imc_vector_concrete_lookup(vec4, 10302);
printf("found at 10302 : %d \n", data_lookup->id);
*/
imc_vector_concrete_emit(vec3, "essaiDOT.dot", print);


}
