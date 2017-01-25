#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "avl_vector.h"

/* Integer data manipulation tools */
typedef struct _int_data_t {
  int content;
} int_data_t;
int_data_t* make_int_data(int i) {
  int_data_t* data = malloc(sizeof *data);
  data->content = i;
  return data;
}
char* data_int_as_string(void* data) {
  char* buf = malloc(20 * sizeof(char)); /* 20 char is enough to old 2**64. */
  sprintf(buf, "%d", ((int_data_t*)data)->content);
  return buf;
}

/* String data manipulation tools */
typedef struct _string_data_t {
  char* content;
} string_data_t;
string_data_t* make_string_data(char* str) {
  string_data_t* data = malloc(sizeof *data);
  data->content = str;
  return data;
}
char* data_string_as_string(void* data) {
  return strdup(((string_data_t*)data)->content);
}


void test_push_pop(int size) {
  avl_vector_t* v = avl_vector_create(data_int_as_string);
  for (int i = 0; i < size; i++) {
    printf("Pushing '%d'\n", i);
    v = avl_vector_push(v, make_int_data(i));
    printf("|%d|  -> ", avl_vector_size(v));
    avl_vector_dump(v);
  }
  while (avl_vector_size(v) != 0) {
    void* data = NULL;
    v = avl_vector_pop(v, &data);
    if (data != NULL) {
      printf("Poping, got '%s'\n", data_int_as_string(data));
    } else {
      printf("Poping, got nothing.\n");
    }
    printf("|%d|  -> ", avl_vector_size(v));
    avl_vector_dump(v);
  }
}

void test_lookup_pop(int size) {
  avl_vector_t* v = avl_vector_create(data_int_as_string);
  for (int i = 0; i < size; i++) {
    int to_insert = rand() % (size * 3);
    printf("Updating v[%d] = %d\n", to_insert, i);
    v = avl_vector_update(v, to_insert, make_int_data(i));
    printf("|%d|  -> ", avl_vector_size(v));
    avl_vector_dump(v);
  }

  while (avl_vector_size(v) != 0) {
    void* data = NULL;
    v = avl_vector_pop(v, &data);
    if (data != NULL) {
      printf("Poping, got '%s'\n", data_int_as_string(data));
    } else {
      printf("Poping, got nothing.\n");
    }
    printf("|%d|  -> ", avl_vector_size(v));
    avl_vector_dump(v);
  }
}

void test_merge_with_push(int size) {
  avl_vector_t* v = avl_vector_create(data_int_as_string);
  avl_vector_t* t = avl_vector_create(data_int_as_string);
  for (int i = 0; i < 2 * size; i+=2) v = avl_vector_push(v, make_int_data(i));
  for (int i = 1; i < 2 * size; i+=2) t = avl_vector_push(t, make_int_data(i));
  printf("About to merge,\n");
  printf("t = ");avl_vector_dump(v);
  printf("v = ");avl_vector_dump(t);

  avl_vector_t* new = avl_vector_merge(v, t);
  printf("result = "); avl_vector_dump(new);
}

void test_merge_with_update(int size) {
  avl_vector_t* v = avl_vector_create(data_int_as_string);
  avl_vector_t* t = avl_vector_create(data_int_as_string);
  for (int i = 0; i < size; i+=2) v = avl_vector_update(v, rand()%(size*3), make_int_data(i));
  for (int i = 1; i < size; i+=2) t = avl_vector_update(t, rand()%(size*3), make_int_data(i));
  printf("About to merge,\n");
  printf("t = ");avl_vector_dump(v);
  printf("v = ");avl_vector_dump(t);

  avl_vector_t* new = avl_vector_merge(v, t);
  printf("result = "); avl_vector_dump(new);
}

void test_split(int size) {
  avl_vector_t* v = avl_vector_create(data_int_as_string);
  for (int i = 0; i < size; i++) {
    v = avl_vector_push(v, make_int_data(i));
  }
  printf("Before split: \n v = ");
  avl_vector_dump(v);
  printf("Spliting on %d.\n", size/2-1);
  avl_vector_t *v1, *v2;
  avl_vector_split(v, size/2-1, &v1, &v2);
  printf("After:\nv1 = ");
  avl_vector_dump(v1);
  printf("v2 = ");
  avl_vector_dump(v2);
}

void test_unref(int size) {
  avl_vector_t* v = avl_vector_create(data_int_as_string);
  for (int i = 0; i < size; i++) {
    avl_vector_t* t = avl_vector_push(v, make_int_data(i));
    avl_vector_unref(v);
    v = t;
  }
  printf("After successive insertions (with unref of previous versions):\nv = ");
  avl_vector_dump(v);
}


void test_genericity() {
  avl_vector_t* int_vec = avl_vector_create(data_int_as_string);
  avl_vector_t* str_vec = avl_vector_create(data_string_as_string);
  int_vec = avl_vector_push(int_vec, make_int_data(1));
  int_vec = avl_vector_push(int_vec, make_int_data(2));
  int_vec = avl_vector_push(int_vec, make_int_data(3));
  int_vec = avl_vector_push(int_vec, make_int_data(4));
  int_vec = avl_vector_push(int_vec, make_int_data(5));

  str_vec = avl_vector_push(str_vec, make_string_data("one"));
  str_vec = avl_vector_push(str_vec, make_string_data("two"));
  str_vec = avl_vector_push(str_vec, make_string_data("three"));
  str_vec = avl_vector_push(str_vec, make_string_data("four"));
  str_vec = avl_vector_push(str_vec, make_string_data("five"));

  printf("Integer vector : "); avl_vector_dump(int_vec);
  printf("\nString vector : "); avl_vector_dump(str_vec);
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

  printf("\n*************************\nTesting SPLIT :\n\n");
  test_split(10);

  printf("\n*************************\nTesting UNREF :\n\n");
  test_unref(10);

  printf("\n*************************\nTesting the genericity :\n\n");
  test_genericity();

  printf("\n\n");
  
  return 0;
}
