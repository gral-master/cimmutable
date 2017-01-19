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
  char* buf = malloc(15 * sizeof(char)); /* 15? because. */
  sprintf(buf, "%d", data->content);
  return buf;
}


void test_push_pop(int size) {
  avl_vector_t* v = avl_vector_create();
  for (int i = 0; i < size; i++) {
    printf("Pushing '%d'\n", i);
    v = avl_vector_push(v, make_data(i));
    printf("|%d|  -> ", avl_vector_size(v));
    avl_vector_dump(v);
  }
  while (avl_vector_size(v) != 0) {
    vector_data_t* data = NULL;// = malloc(sizeof *data);
    v = avl_vector_pop(v, &data);
    if (data != NULL) {
      printf("Poping, got '%s'\n", data_as_string(data));
    } else {
      printf("Poping, got nothing.\n");
    }
    printf("|%d|  -> ", avl_vector_size(v));
    avl_vector_dump(v);
  }
}

void test_lookup_pop(int size) {
  avl_vector_t* v = avl_vector_create();
  for (int i = 0; i < size; i++) {
    int to_insert = rand() % (size * 3);
    printf("Updating v[%d] = %d\n", to_insert, i);
    v = avl_vector_update(v, to_insert, make_data(i));
    printf("|%d|  -> ", avl_vector_size(v));
    avl_vector_dump(v);
  }

  while (avl_vector_size(v) != 0) {
    vector_data_t* data = NULL;// = malloc(sizeof *data);
    v = avl_vector_pop(v, &data);
    if (data != NULL) {
      printf("Poping, got '%s'\n", data_as_string(data));
    } else {
      printf("Poping, got nothing.\n");
    }
    printf("|%d|  -> ", avl_vector_size(v));
    avl_vector_dump(v);
  }
}

void test_merge_with_push(int size) {
  avl_vector_t* v = avl_vector_create();
  avl_vector_t* t = avl_vector_create();
  for (int i = 0; i < 2 * size; i+=2) v = avl_vector_push(v, make_data(i));
  for (int i = 1; i < 2 * size; i+=2) t = avl_vector_push(t, make_data(i));
  printf("About to merge,\n");
  printf("t = ");avl_vector_dump(v);
  printf("v = ");avl_vector_dump(t);

  avl_vector_t* new = avl_vector_merge(v, t);
  printf("result = "); avl_vector_dump(new);
}

void test_merge_with_update(int size) {
  avl_vector_t* v = avl_vector_create();
  avl_vector_t* t = avl_vector_create();
  for (int i = 0; i < size; i+=2) v = avl_vector_update(v, rand()%(size*3), make_data(i));
  for (int i = 1; i < size; i+=2) t = avl_vector_update(t, rand()%(size*3), make_data(i));
  printf("About to merge,\n");
  printf("t = ");avl_vector_dump(v);
  printf("v = ");avl_vector_dump(t);

  avl_vector_t* new = avl_vector_merge(v, t);
  printf("result = "); avl_vector_dump(new);
}

void avl_various_test() {
  int n = 10;
  avl_vector_t* t[n];
  t[0] = avl_vector_create();
    
  for (int i = 0; i < n; i++)  {
    t[i+1] = avl_vector_update(t[i], i, make_data(i));
  }

  avl_vector_dump(t[n]);
}

int main () {

  srand(10);

  printf("\n*************************\nTesting PUSH - POP  :\n\n");
  test_push_pop(10);
  

  printf("\n*************************\nTesting LOOKUP - POP  :\n\n");
  test_lookup_pop(10);

  printf("\n*************************\nTesting MERGE (after PUSHes):\n\n");
  test_merge_with_push(10);

  printf("\n*************************\nTesting MERGE (after UPDATEs)  :\n\n");
  test_merge_with_update(10);


  printf("\n\n");
  
  return 0;
}
