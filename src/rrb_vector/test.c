#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include "./src/rrb_vector/rrb_vect.h"
//Compile with gcc test.c obj/*.o -lm

int next_rand(int a, int b) {
    return rand() % (b + 1 - a) + a;
}

void print(imc_data_t* data) {
    printf("data : %d\n", data->id);
}

char* print_v1(imc_data_t* data) {
    char *str = malloc(sizeof(char) * 15);
    sprintf(str, "%d", data->id);
    return str;
}

int main(){
    time_t t;
    int i, r;
    
    struct timeval tm1;
    srand((unsigned) time(&t));
    
    imc_rrb_t* vec1, *vec2;
    imc_data_t** data = malloc(sizeof(imc_data_t*) * 2000);
    vec1 = imc_rrb_create();
    
    puts("\nPUT\n");
    for(i = 0; i<2000; i++) {
        data[i] = malloc(sizeof(imc_data_t));
        data[i]->id = next_rand(0, 20);
        vec2 = imc_rrb_push(vec1, data[i]);
        r = imc_rrb_unref(vec1);
        vec1 = vec2;
    }
    

    
    gettimeofday(&tm1, NULL);
    
    for(i = 0; i < 20000; i++){
        vec2 = imc_rrb_pop(vec1, &data[i]);
        //r = imc_rrb_unref(vec1);
        vec1 = vec2;
    }

    struct timeval tm2;
    gettimeofday(&tm2, NULL);
    
    unsigned long long t2 = 1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000;
    printf("%llu ms\n", t2);

    
    //imc_rrb_t* vec_left = imc_rrb_create();
    //imc_rrb_t* vec_right = imc_rrb_create();
    //imc_rrb_split(vec1, 1000, &vec_left, &vec_right);
    

    
    //imc_rrb_emit(vec1, "./dot/before.dot", print_v1);
    
    
    //imc_rrb_emit(vec_left, "./dot/left.dot", print_v1);
    //imc_rrb_emit(vec_right, "./dot/right.dot", print_v1);
    
    //imc_rrb_t* merged = imc_rrb_merge(vec_left, vec_right);
    //imc_rrb_emit(merged, "./dot/merged.dot", print_v1);
    
    r = imc_rrb_unref(vec1);
    //if(r == 1) printf("Freed\n");
    //r = imc_rrb_unref(vec_left);
    //if(r == 1) printf("Freed\n");
    //r = imc_rrb_unref(vec_right);
    //if(r == 1) printf("Freed\n");
    //r = imc_rrb_unref(merged);
    //if(r == 1) printf("Freed\n");
    for(i = 0; i < 2000; i++) free(data[i]);
    free(data);
   
    return EXIT_SUCCESS;
}
