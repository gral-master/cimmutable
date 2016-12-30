
#include <stdlib.h>
#include <stdio.h>
#include "imc_avl.h"



int compare (int* a , int* b)
{
	return *a - *b;
}

void print (int* nb, char* b)
{

	sprintf(b, "(%03d)", *nb);
}

int main ()
{
	imc_data_t* replace;

	imc_data_t data[6] = {0, 1, 2, 3, 4, 5};
	imc_key_t key[6] = {10, 4, 6, 12, 14, 8};

	imc_avl_node_t* tree = NULL;
    int i;
	for (i = 0 ; i < 6 ; i++)
	{
        replace = NULL;
		tree = imc_avl_insert(tree, &data[i], &key[i], &compare, &replace);
		imc_avl_dump(tree, print);
	}

	replace = NULL;
	tree = imc_avl_remove(tree, &key[0], &compare, &replace);
    printf("tree->balance = %d\n", tree->balance);
	imc_avl_dump(tree, print);

    replace = NULL;
    tree = imc_avl_remove(tree, &key[3], &compare, &replace);
    printf("tree->balance = %d\n", tree->balance);
    imc_avl_dump(tree, print);

	//printf("%d\n", (int) *result);

	return 0;
}
