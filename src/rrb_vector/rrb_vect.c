#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include "../debug.h"
#include "./rrb_vect.h"



/* TODO -> invariant , pre/post cond */
int imc_vector_concrete_size(imc_rrb_t* vec) {
    return vec -> element_count;
}

imc_rrb_t* imc_vector_concrete_update(imc_rrb_t* vec, int index, imc_data_t* data) {
  /* preconditions */

  /* invariant */

  /* algorithm */
  puts("\nUpdate in a vector : ");
  imc_rrb_t* new_root;
  new_root = imc_vector_concrete_copy(vec);
  vec = new_root;

  int sub_index;

  while(vec->level != 1) {
     sub_index = imc_vector_concrete_subindex(vec, index);
     if(vec->children.subtrees[sub_index] != NULL) {
       vec->children.subtrees[sub_index]->refs -= 1; //has been up by previous copy, we need to do -1 because we don't reference it in our new copy
       vec->children.subtrees[sub_index] = imc_vector_concrete_copy(vec->children.subtrees[sub_index]); // This function updates refs to children
     } else {
       //Should not happen
       //TODO : Free previous
       return vec;
     }
     printf("Update to the vector of %d elems of level %d at index %d, taken path : %d\n", vec->element_count, vec->level, index, sub_index);
     vec = vec->children.subtrees[sub_index];
  }

  //Here, vec is the first floor
  sub_index = imc_vector_concrete_subindex(vec, index);
  printf("Update to the vector of %d elems of level %d at index %d, taken path : %d\n", vec->element_count, vec->level, index, sub_index);

  if(vec->children.subtrees[sub_index]!=NULL) {
    vec->children.subtrees[sub_index] = imc_vector_concrete_copy_leaf(vec->children.subtrees[sub_index]); //This should also copy the data
  } else {
    //Should not happen
    //TODO : Free previous
    return vec;
  }

  vec = vec->children.subtrees[sub_index];
  printf("Update to the vector of %d elems of level %d at index %d, taken path : %d\n", vec->element_count, vec->level, index, sub_index);
  //Here, vec is a fresh leaf with data inside
  //sub_index = imc_vector_concrete_subindex(vec, new_root->element_count); // On veut ajouter à l'indice nb_element
  sub_index = imc_vector_concrete_subindex(vec, index);
  vec->children.data[sub_index] = data;
  printf("Updated to the data at index %d, taken path : %d\n", index, sub_index);

  return new_root;
}

imc_data_t* imc_vector_concrete_lookup(imc_rrb_t* vec, int index) {
  printf("lookup in a vector of size %d at index %d\n", vec->element_count, index);
  /* preconditions */
  if(index<0) {
    LOG(LOG_ERR, "Illegal access attempt with negative index %d.", index);
    return NULL;
  }
  if(vec==NULL) {
    //TODO : Regarder la dernière case de la meta, pour savoir si on va faire un accès trop grand
    LOG(LOG_ERR, "Illegal access attempt with positive index %d || Illegal lookup attempt in a NULL vector.", index);
    return NULL;
  }
  /* invariant */
  if(vec->level < 1) {
    LOG(LOG_FATAL, "Illegal data structure state : negative level %d.", vec->level);
    return NULL;
  }
  /* algorithm */
  int level_index = 0;
  while(vec->level!=0) {
    level_index = imc_vector_concrete_subindex(vec, index);
    if(level_index != -1) {
      vec = vec->children.subtrees[level_index];
    } else {
      puts("index cannot be reached.\n");
      return NULL;
    }
  }
  level_index = imc_vector_concrete_subindex(vec, index);
  return vec->children.data[level_index];
}

/* stack operations */
imc_rrb_t* imc_vector_concrete_push_full(imc_rrb_t* vec, imc_data_t* data) {
  puts("\nInsertion in full vector : ");
  imc_rrb_t* new_root;
  //New root with vec as first child
  new_root = imc_vector_concrete_new_root(vec);
  //Create the path to add data
  new_root->children.subtrees[1] = imc_vector_concrete_create();
  vec = new_root->children.subtrees[1];
  vec->level = new_root->level - 1;

  printf("Insertion to the vector of %d elems of level %d at index %d, taken path : 1\n", new_root->element_count, new_root->level, new_root->element_count);
  while(vec->level != 0) {
    printf("Insertion to the vector of %d elems of level %d at index %d, taken path : 0\n", vec->element_count, vec->level, new_root->element_count);
    vec->children.subtrees[0] = imc_vector_concrete_create();
    vec->children.subtrees[0]->level = vec->level - 1;
    vec->element_count = 1;
    vec = vec->children.subtrees[0];
  }
  vec->element_count = 1;
  vec->children.subtrees[0] = imc_vector_concrete_create_leaf();
  vec = vec->children.subtrees[0];
  printf("Insertion to the vector of %d elems of level %d at index %d, taken path : 0\n", vec->element_count, vec->level, new_root->element_count);
  vec->children.data[0] = data;
  printf("Inserted to the data at index %d, taken path : 0\n", new_root->element_count);
  new_root->element_count += 1;
  return new_root;
}

imc_rrb_t* imc_vector_concrete_push_not_full(imc_rrb_t* vec, imc_data_t* data) {
    puts("\nInsertion in not full vector : ");
    imc_rrb_t* new_root;
    // new version of the root
    new_root = imc_vector_concrete_copy(vec);
    vec = new_root;


    int insert_index = vec->element_count;
    int sub_index;

    while(vec->level != 1) {
       sub_index = imc_vector_concrete_subindex(vec, insert_index);
       if(vec->children.subtrees[sub_index] != NULL) {
         vec->children.subtrees[sub_index]->refs -= 1; //has been up by previous copy, we need to do -1 because we don't reference it in our new copy
         vec->children.subtrees[sub_index] = imc_vector_concrete_copy(vec->children.subtrees[sub_index]); // This function updates refs to children
       } else {
         vec->children.subtrees[sub_index] = imc_vector_concrete_create();
       }
       printf("Insertion to the vector of %d elems of level %d at index %d, taken path : %d\n", vec->element_count, vec->level, insert_index, sub_index);
       vec->element_count += 1;
       vec = vec->children.subtrees[sub_index];
       // /!\ What if the path has not been created yet ? vec could be NULL ? :-(
    }


    //Here, vec is the first floor
    sub_index = imc_vector_concrete_subindex(vec, insert_index);
    printf("Insertion to the vector of %d elems of level %d at index %d, taken path : %d\n", vec->element_count, vec->level, insert_index, sub_index);

    if(vec->children.subtrees[sub_index]!=NULL) {
      vec->children.subtrees[sub_index] = imc_vector_concrete_copy_leaf(vec->children.subtrees[sub_index]); //This should also copy the data
    } else {
      vec->children.subtrees[sub_index] = imc_vector_concrete_create_leaf();
    }
    vec->element_count += 1;
    vec = vec->children.subtrees[sub_index];
    printf("Insertion to the vector of %d elems of level %d at index %d, taken path : %d\n", vec->element_count, vec->level, insert_index, sub_index);
    //Here, vec is a fresh leaf with data inside
    //sub_index = imc_vector_concrete_subindex(vec, new_root->element_count); // On veut ajouter à l'indice nb_element
    sub_index = imc_vector_concrete_subindex(vec, insert_index);
    vec->children.data[sub_index] = data;
    vec->element_count += 1;
    printf("Inserted to the data at index %d, taken path : %d\n", insert_index, sub_index);

    return new_root;
}
// add at the end <--- DOXYGENIZE PLEASE!
imc_rrb_t* imc_vector_concrete_push(imc_rrb_t* vec, imc_data_t* data) {

  /* preconditions */
  if(data==NULL) {
    LOG(LOG_ERR, "Illegal push attempt NULL data.");
    return NULL;
  }
  if(vec==NULL) {
    //TODO : Regarder la dernière case de la meta, pour savoir si on va faire un accès trop grand
    LOG(LOG_ERR, "Illegal push attempt in a NULL vector.");
    return NULL;
  }
  /* invariant */
  if(vec->level < 1) {
    LOG(LOG_FATAL, "Illegal data structure state : negative level %d.", vec->level);
    return NULL;
  }

  //algorithm

  if(imc_vector_concrete_full(vec)==1) {
    return imc_vector_concrete_push_full(vec, data);
  } else {
    return imc_vector_concrete_push_not_full(vec, data);
  }
}

imc_rrb_t* imc_vector_concrete_pop(imc_rrb_t* vec, imc_data_t** data) {
  //TODO : Si la branche devient inutile, il faut la supprimer. Idem pour un étage, ça ne coûte pas cher !
  //TODO : Il faut faire les refs.
  //TODO : Il faut faire les free !
  /* preconditions */

  /* invariant */

  /* algorithm */

  // 1) We go to the desire data, copying the path
  // 2) At floor 1, we copy the data node if and only if the looking data is not in first position
  // 3) While copying the data, we remove the last value
  if(vec->element_count<=0) {
    puts("vector is already empty !\n");
    return vec;
  }
  imc_rrb_t* new_root = imc_vector_concrete_copy(vec);
  vec = new_root;
  int sub_index = 0;
  int last_index = new_root->element_count-1;
  while(vec->level!=1) {
    sub_index = imc_vector_concrete_subindex(vec, last_index); // We want the last elem
    vec->children.subtrees[sub_index] = imc_vector_concrete_copy(vec->children.subtrees[sub_index]); // This function updates refs to children
    printf("Pop in a vector of size %d at level %d, taken : %d \n", vec->element_count, vec->level, sub_index);
    vec->element_count -= 1;
    vec = vec->children.subtrees[sub_index];
  }
  //level 1 : vec's child is a leaf.
  sub_index = imc_vector_concrete_subindex(vec, last_index);
  vec->children.subtrees[sub_index] = imc_vector_concrete_copy_leaf(vec->children.subtrees[sub_index]); // This should also copy the data
  printf("Pop in a vector of size %d at level %d, taken : %d \n", vec->element_count, vec->level, sub_index);
  vec->element_count -= 1;
  vec = vec->children.subtrees[sub_index];
  //level 0 : vec is a leaf
  sub_index = imc_vector_concrete_subindex(vec, last_index);
  printf("Extracted data in a vector of size %d at level %d, taken : %d \n", vec->element_count, vec->level, sub_index);
  *data = vec->children.data[sub_index];
  vec->children.data[sub_index] = NULL;
  vec->element_count -= 1;
  return new_root;

}

int imc_vector_concrete_split(imc_rrb_t* vec_in, int index, imc_rrb_t** vec_out1, imc_rrb_t** vec_out2) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return -1;
}

imc_rrb_t* imc_vector_concrete_merge(imc_rrb_t* vec_front, imc_rrb_t* vec_tail) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return NULL;
}

/* user-side memory management */

int imc_vector_concrete_unref(imc_rrb_t* vec) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return -1;
}

void imc_vector_concrete_dump(imc_rrb_t* vec) {
  /* preconditions */

  /* invariant */

  /* algorithm */

  return;
}

/* utils */
void imc_vector_concrete_emit(imc_rrb_t* vec, const char* path, char* (*print)(imc_data_t*)) {
  /* preconditions */

  /* invariant */

  /* algorithm */
  FILE *fp;
  fp = fopen(path, "w+");
  //initialisation :
  fprintf(fp, "digraph g {\n");
  fprintf(fp, "node [shape = record,height=.1];\n");
  emit_node(vec, NULL, "", fp, print);
  fprintf(fp, "}\n");
  fclose(fp);
}

//On pourrait faire un emit_node_compact ?
void emit_node(imc_rrb_t* vec, char* from, char* prefix, FILE* fp, char* (*print)(imc_data_t*)) {
  /* added i (as identifier) to the prefix, because of a graphviz bug :
   * https://rt.cpan.org/Public/Bug/Display.html?id=105171
   * Please use after that dot -Tsvg INPUT.dot -o OUTPUT.svg
   * and use a good visualizer if your graph is large.
   */

  fprintf(fp, "node_%s[label = \"", prefix);
  //Pas exactement pareil si c'est une feuille
  char suffix;
  if(vec->level != 0) {
    for(int i =0; i<ARRAY_SIZE; i++) {
      if(vec->children.subtrees[i] != NULL) {
        suffix = i<10?i+48:i+55;
        printf("prefix : %s\n", prefix);
        printf("suffix : %c\n", suffix);
        fprintf(fp, "<i%s> %d", concatc(prefix, suffix), i);
        if(i != ARRAY_SIZE-1 && vec->children.subtrees[i+1] != NULL) {
          fprintf(fp, "| ");
        }
      }
    }
    fprintf(fp, "\"];\n");

    if(from != NULL) {
      fprintf(fp, "%s -> \"node_%s\":i%s0;\n", from, prefix, prefix);
    }
    //"node0":f2 -> "node4":f1;
    for(int i =0; i<ARRAY_SIZE; i++) {
      if(vec->children.subtrees[i] != NULL) {
        suffix = i<10?i+48:i+55;
        char* str_from = malloc(sizeof(char) * 100);
        sprintf(str_from, "\"node_%s\":i%s", prefix, concatc(prefix, suffix));
        emit_node(vec->children.subtrees[i], str_from, concatc(prefix, suffix), fp, print);
      }
    }
  } else {
      for(int i =0; i<ARRAY_SIZE; i++) {
        if(vec->children.data[i] != NULL) {
          suffix = i<10?i+48:i+55;
          fprintf(fp, "<i%s> %s ", concatc(prefix, suffix), print(vec->children.data[i]));
          if(i != ARRAY_SIZE-1 && vec->children.subtrees[i+1] != NULL) {
            fprintf(fp, "|");
          }
        }
      }
      fprintf(fp, "\"];\n");
      if(from != NULL) {
        fprintf(fp, "%s -> \"node_%s\":i%s0;\n", from, prefix, prefix);
      }
  }
}

char* concatc(char* str, char c) {
    size_t len = strlen(str);
    char *str2 = malloc(len + 1 + 1 );
    strcpy(str2, str);
    str2[len] = c;
    str2[len + 1] = '\0';
    return str2;
}

/* return 1 if the vector is full */
int imc_vector_concrete_full(imc_rrb_t* vec) {
  if(imc_vector_concrete_balanced(vec) == 1) {
    return (vec->element_count == (32*pow(ARRAY_SIZE, vec->level))) ? 1 : 0;
  } else {
    while(vec->level > 1) {
      if(vec->meta[ARRAY_SIZE-1]==0) {
        return 0;
      }
    }
    vec = vec->children.subtrees[ARRAY_SIZE-1];
    return (vec->children.data[ARRAY_SIZE-1]!=NULL) ? 1 : 0;
  }
}

/* Add a new root to the top of a vector */
imc_rrb_t* imc_vector_concrete_new_root(imc_rrb_t* vec) {
  imc_rrb_t* new_root = NULL;
  new_root = imc_vector_concrete_create();
  new_root->children.subtrees[0] = vec;
  vec->refs+=1;
  new_root->element_count = vec->element_count;
  new_root->level = vec->level+1;
  return new_root;
}

/* return 1 if the vector is balanced */
int imc_vector_concrete_balanced(imc_rrb_t* vec) {
  /* /!\ Is that always true ? What if the rrb has been rebalanced ? /!\ */
  return (vec->meta==NULL) ? 1 : 0;
}

/* return the subindex, i.e. the subindex you may choose at your current
   level to go to the vector index */
int imc_vector_concrete_subindex(imc_rrb_t* vec, int index) {
  int level_index = 0;
  if(imc_vector_concrete_balanced(vec)==1) { //Si on est balanced, on sait tout de suite où aller
    level_index = (index >> (int)(log2((double)ARRAY_SIZE) * vec->level)) & (ARRAY_SIZE-1);
  } else { //Si on est unbalanced, on doit chercher où aller
    while(vec->meta[level_index] <= index) {
      level_index++;
      if(level_index==ARRAY_SIZE) { //We didn't find it
        return -1;
      }
    }
  }
  return level_index;
}

imc_rrb_t* imc_vector_concrete_copy_leaf(imc_rrb_t* vec) {
  imc_rrb_t* vec_copy = imc_vector_concrete_create_leaf();
  vec_copy -> level = vec -> level;
  vec_copy -> element_count =  vec -> element_count;
  vec_copy -> meta = vec -> meta; // A changer en allouant un nouveau tableau meta
  if(vec -> children.data != NULL) {
    vec_copy -> children.data = malloc(sizeof(imc_data_t*) * ARRAY_SIZE);
    for(int i = 0; i < ARRAY_SIZE ; i++) {
      vec_copy -> children.data[i] = vec -> children.data[i];
    }
  }
  return vec_copy;
}

imc_rrb_t* imc_vector_concrete_copy(imc_rrb_t* vec) {
  imc_rrb_t* vec_copy = imc_vector_concrete_create();
  vec_copy -> level = vec -> level;
  vec_copy -> element_count =  vec -> element_count;
  vec_copy -> meta = vec -> meta; // A changer en allouant un nouveau tableau meta
  vec_copy -> refs = 1; //It is a copy, so we have one and only one ref to it
  if(vec -> children.subtrees != NULL) {
    vec_copy -> children.subtrees = malloc(sizeof(imc_rrb_t*) * ARRAY_SIZE);
    for(int i = 0; i < ARRAY_SIZE ; i++) {
      vec_copy -> children.subtrees[i] = vec -> children.subtrees[i];
      if(vec_copy -> children.subtrees[i] != NULL) {
        vec_copy -> children.subtrees[i] -> refs += 1;
      }
    }
  }

  return vec_copy;
}

imc_rrb_t* imc_vector_concrete_create_leaf() {
  imc_rrb_t* vec = malloc(sizeof(imc_rrb_t));

  if(vec == NULL){
      LOG(LOG_FATAL, "Allocation failure %s", strerror(errno));
      return NULL;
  }
  vec -> level = 0;
  vec -> refs = 1;
  vec -> element_count = 0;
  vec -> meta = NULL;
  vec -> children.data = malloc(sizeof(imc_data_t*) * ARRAY_SIZE);

  if(vec -> children.data == NULL){
      LOG(1, "Allocation failure %s", strerror(errno));
      free(vec);
      return NULL;
  }

  for(int i = 0; i<ARRAY_SIZE; i++) {
    vec -> children.data[i] = NULL;
  }
  return vec;
}

/* --> rrb_vect.h */
/* TODO -> invariant , pre/post cond */
imc_rrb_t* imc_vector_concrete_create() {
    imc_rrb_t* vec = malloc(sizeof(imc_rrb_t));

    if(vec == NULL){
        LOG(LOG_FATAL, "Allocation failure %s", strerror(errno));
        puts("Erreur allocation ! \n");
        return NULL;
    }
    vec -> level = 1;
    vec -> refs = 1;
    vec -> element_count = 0;
    vec -> meta = NULL;
    vec -> children.subtrees = malloc(sizeof(imc_rrb_t*) * ARRAY_SIZE);

    if(vec -> children.subtrees == NULL){
        LOG(1, "Allocation failure %s", strerror(errno));
        free(vec);
        return NULL;
    }
    for(int i = 0; i<ARRAY_SIZE; i++) {
      vec-> children.subtrees[i] = NULL;
    }

    return vec;
}
