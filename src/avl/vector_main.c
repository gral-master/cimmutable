#include <stdio.h>
#include <stdlib.h>

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
  char* buf = malloc(15 * sizeof(char));
  sprintf(buf, "%d", data->content);
  return buf;
}

int main () {
  int n = 10;
  avl_vector_t* t[n];
  t[0] = avl_vector_create();
    
  for (int i = 0; i < n; i++)  {
    t[i+1] = avl_vector_update(t[i], i, make_data(i));
  }

  avl_vector_dump(t[n]);

  return 0;
}
