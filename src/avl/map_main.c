#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl_map.h"


int main () {

  srand(10);

  avl_map_t* map = avl_map_create(string_box_as_string,
				  int_box_as_string,
				  compare_string_keys);
  
  printf("First version:\n");
  map = avl_map_update(map, make_string_box("First"), make_int_box(1));
  map = avl_map_update(map, make_string_box("Second"), make_int_box(2));
  map = avl_map_update(map, make_string_box("Fourth"), make_int_box(3));
  map = avl_map_update(map, make_string_box("Fifth"), make_int_box(4));
  map = avl_map_update(map, make_string_box("Sixth"), make_int_box(5));
  avl_map_dump(map);
  
  printf("\nNow updating First, Second and Sixth:\n");
  map = avl_map_update(map, make_string_box("First"), make_int_box(19));
  map = avl_map_update(map, make_string_box("Second"), make_int_box(41));
  map = avl_map_update(map, make_string_box("Sixth"), make_int_box(25));
  avl_map_dump(map);
  avl_map_t* backup = map;
  
  printf("\nNow removing First, Fifth and Second\n");
  void* data = NULL;
  map = avl_map_remove(map, make_string_box("First"), &data);
  map = avl_map_remove(map, make_string_box("Fifth"), &data);
  map = avl_map_remove(map, make_string_box("Second"), &data);
  avl_map_dump(map);
  
  printf("\nVerification of immutability: Restauring the old map:\n");
  map = backup;
  avl_map_dump(map);
  
  printf("\nGetting the list of keys :\n");
  void** keys = avl_map_keys(map);
  printf("Keys = [ ");
  for (int i = 0; i < avl_map_size(map); i++) {
    printf("%s, ", *((string_box_t*)keys[i]));
  }
  printf("\b\b ]\n");

  printf("\nNow testing the iterator :\n");
  map_iterator_t* iterator = avl_map_create_iterator(map);
  void* key = NULL;
  data = NULL;
  while ( avl_map_iterator_next(&iterator, &key, &data) != 0 ) {
    printf(" -> map{%s} = %d\n", *((string_box_t*)key), *((int_box_t*)data));
  }

  printf("\nCreating an int-char map to test genericity\n");
  avl_map_t* map2 = avl_map_create(int_box_as_string,
				   string_box_as_string,
				   compare_string_keys);
  map2 = avl_map_update(map2, make_int_box(1), make_string_box("First"));
  map2 = avl_map_update(map2, make_int_box(2), make_string_box("Second"));
  map2 = avl_map_update(map2, make_int_box(3), make_string_box("Fourth"));
  map2 = avl_map_update(map2, make_int_box(4), make_string_box("Fifth"));
  avl_map_dump(map2);
  
  printf("\n\n");
  
  return 0;
}
