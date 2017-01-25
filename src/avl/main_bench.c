#include <stdio.h>
#include <stdlib.h>

#include "avl_vector.h"

/* Needed for the vectors, don't mind the following definitions */
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



int main(int argc, char* argv[]) {
  
}
