#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl_map.h"

/* int box */
typedef struct _int_box_t {
  int content;
} int_box_t;
int_box_t* make_int_box(int i) {
  int_box_t* data = malloc(sizeof *data);
  data->content = i;
  return data;
}
char* int_box_as_string(void* data) {
  char* buf = malloc(20 * sizeof(char)); /* 20 char is enough to old 2**64. */
  sprintf(buf, "%d", ((int_box_t*)data)->content);
  return buf;
}

/* char* box */
typedef struct _char_box_t {
  char* content;
} string_box_t;
string_box_t* make_char_box(char* str) {
  string_box_t* ret = malloc(sizeof *ret);
  ret->content = str;
  return ret;
}
char* string_box_as_string(void* box) {
  return strdup(((string_box_t*)box)->content);
}


/* compare strings */
int key_compare_string(void* k1, void* k2) {
  return strcmp(((string_box_t*)k1)->content, ((string_box_t*)k2)->content);
}



int main () {

  srand(10);

  avl_map_t* map = avl_map_create(string_box_as_string,
				  int_box_as_string,
				  key_compare_string);
  
  printf("First version:\n");
  map = avl_map_update(map, make_char_box("First"), make_int_box(1));
  map = avl_map_update(map, make_char_box("Second"), make_int_box(2));
  map = avl_map_update(map, make_char_box("Fourth"), make_int_box(3));
  map = avl_map_update(map, make_char_box("Fifth"), make_int_box(4));
  map = avl_map_update(map, make_char_box("Sixth"), make_int_box(5));
  avl_map_dump(map);
  
  printf("\nNow updating First, Second and Sixth:\n");
  map = avl_map_update(map, make_char_box("First"), make_int_box(19));
  map = avl_map_update(map, make_char_box("Second"), make_int_box(41));
  map = avl_map_update(map, make_char_box("Sixth"), make_int_box(25));
  avl_map_dump(map);
  avl_map_t* backup = map;
  
  printf("\nNow removing First, Fifth and Second\n");
  void* data = NULL;
  map = avl_map_remove(map, make_char_box("First"), &data);
  map = avl_map_remove(map, make_char_box("Fifth"), &data);
  map = avl_map_remove(map, make_char_box("Second"), &data);
  avl_map_dump(map);
  
  printf("\nVerification of immutability: Restauring the old map:\n");
  map = backup;
  avl_map_dump(map);
  
  printf("\nGetting the list of keys :\n");
  void** keys = avl_map_keys(map);
  printf("Keys = [ ");
  for (int i = 0; i < avl_map_size(map); i++) {
    printf("%s, ", ((string_box_t*)keys[i])->content);
  }
  printf("\b\b ]\n");

  printf("\nNow testing the iterator :\n");
  map_iterator_t* iterator = avl_map_create_iterator(map);
  void* key = NULL;
  data = NULL;
  while ( avl_map_iterator_next(&iterator, &key, &data) != 0 ) {
    printf(" -> map{%s} = %d\n", ((string_box_t*)key)->content, ((int_box_t*)data)->content);
  }

  printf("\nCreating an int-char map to test genericity\n");
  avl_map_create(int_box_as_string,
		 string_box_as_string,
		 key_compare_string);

  printf("\n\n");
  
  return 0;
}
