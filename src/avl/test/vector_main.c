#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "avl_vector.h"

struct _vector_data_t {
  int content;
};
vector_data_t* make_data(int i) {
  vector_data_t* data = malloc(sizeof *data);
  data->content = i;
  return data;
}
char* data_as_string(vector_data_t* data) {
  char* buf = malloc(20 * sizeof(char)); /* 20 char is enough to old 2**64. */
  sprintf(buf, "%d", data->content);
  return buf;
}

void test_unref(int size) {
  avl_vector_t* v = avl_vector_create();
  for (int i = 0; i < size; i++) {
    avl_vector_t* t = avl_vector_push(v, make_data(i));
    avl_vector_unref(v);
    v = t;
  }
  printf("After successive insertions (with unref of previous versions):\nv = ");
  avl_vector_dump(v);
}



int main () {

  srand(10);

  printf("\n*************************\nTesting UNREF :\n\n");
  test_unref(10);

  printf("\n\n");
  
  return 0;
}
