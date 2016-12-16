#include "imc_avl.h"
#include <stdlib.h>


// TODO : manage the balance attribute
imc_avl_node_t* zig(imc_avl_node_t* tree){
    if(tree == NULL || tree->left == NULL){
        return NULL;
    }

    imc_avl_node_t* new_right = malloc(sizeof(imc_avl_node_t));
    new_right->ref_counter = 1;
    new_right->data = tree->data;
    new_right->key = tree->key;
    new_right->right = tree->right;
    if(tree->right != NULL){
        tree->right->ref_counter++;
    }
    new_right->left = tree->left->right;
    if(tree->left->right != NULL){
        tree->left->right->ref_counter++;
    }

    imc_avl_node_t* new_root = malloc(sizeof(imc_avl_node_t));
    new_root->ref_counter = 1;
    new_root->data = tree->left->data;
    new_root->key = tree->left->key;
    if(tree->left->left != NULL){
        tree->left->left->ref_counter++;
    }
    new_root->left = tree->left->left;
    new_root->right = new_right;

    return new_root;
}

// TODO : manage the balance attribute
imc_avl_node_t* zag(imc_avl_node_t* tree){
    if(tree == NULL || tree->left == NULL){
        return NULL;
    }

    imc_avl_node_t* new_left = malloc(sizeof(imc_avl_node_t));
    new_left->ref_counter = 1;
    new_left->data = tree->data;
    new_left->key = tree->key;
    new_left->left = tree->left;
    if(tree->left != NULL){
        tree->left->ref_counter++;
    }
    new_left->right = tree->right->left;
    if(tree->right->left != NULL){
        tree->right->left->ref_counter++;
    }

    imc_avl_node_t* new_root = malloc(sizeof(imc_avl_node_t));
    new_root->ref_counter = 1;
    new_root->data = tree->right->data;
    new_root->key = tree->right->key;
    if(tree->right->right != NULL){
        tree->right->right->ref_counter++;
    }
    new_root->right = tree->right->right;
    new_root->left = new_left;

    return new_root;
}

//***********************COMPARE FUNCTION*************************************//
int isSup(imc_key_t* x, imc_key_t* y)
{
    return *x > *y;
}
//***********************COMPARE FUNCTION*************************************//




//----------------------------------------------------------------------------//
//-------------------------Lookup Functions-----------------------------------//
//----------------------------------------------------------------------------//
imc_data_t* imc_avl_lookup(imc_avl_node_t* vec, imc_key_t* key,int (*comparaison)(imc_key_t*, imc_key_t*)) {
    if (vec != NULL) {
        if (*vec->key == *key) {
            return vec->data;
        }
        else {
            if (comparaison(key, vec->key)) {
                return imc_avl_lookup(vec->right, key,comparaison);
            }
            else {
                return imc_avl_lookup(vec->left, key,comparaison);
            }
        }
    }
    else {
        return NULL;
    }
}

//----------------------------------------------------------------------------//
//-------------------------Insert Functions-----------------------------------//
//----------------------------------------------------------------------------//

/**
 * Function to do one or two rotation to have a well structure tree
 **/
imc_avl_node_t* imc_avl_insert_do_balance(imc_avl_node_t* vec) {

    if (vec->balance == 2) {
        if (vec->right->balance == 1) {
            return zag(vec);
        }
        else if (vec->right->balance == -1) {
            return zag(zig(vec));
        }
    }
    else {
        if (vec->right->balance == 1) {
            return zig(zag(vec));
        }
        else if (vec->right->balance == -1) {
            return zig(vec);
        }
    }
}


/**
 * Function to update the balance of a node
 **/
void imc_avl_insert_update_balance(imc_avl_node_t* vec) {
    if (vec->right == NULL && vec->left == NULL) {
        vec->balance = 0;
    }
    else {
        if (vec->right == NULL) {
            vec->balance = - abs(vec->left->balance) - 1;
        }

        else if (vec->left == NULL) {
            vec->balance = abs(vec->right->balance) + 1;
        }
        else {
            vec->balance = abs(vec->right->balance) - abs(vec->left->balance);
        }
    }
}

imc_avl_node_t* imc_avl_insert_rec(imc_avl_node_t* vec,
                  imc_data_t* data, imc_key_t* key)
{
    imc_avl_node_t* new_node = malloc(sizeof(imc_avl_node_t));

    if (vec == NULL) {
        // we add a new node without any childrens
        new_node->key = key;
        new_node->data = data;
        new_node->balance = 0;
        new_node->ref_counter = 1;
        return vec;
    }
    else {
        // we recreate the node
        new_node->data = vec->data;
        new_node->key = vec->key;
        new_node->ref_counter = 1;
        if (isSup(key, vec->key)) {
            vec->right = imc_avl_insert_rec(vec->right, data, key);
            vec->left = vec->left;
        }
        else {
            vec->left = imc_avl_insert_rec(vec->left, data, key);
            vec-> right = vec->right;
        }

        imc_avl_insert_update_balance(new_node);
        if (new_node->balance == 2 || new_node->balance == -2) {
            new_node = imc_avl_insert_do_balance(vec);
        }

    }

    return new_node;
}

imc_avl_node_t* imc_avl_insert(imc_avl_node_t* vec,
                  imc_data_t* data, imc_key_t* key) {
    if (imc_avl_lookup(vec, key,isSup) != NULL) {
        return NULL;
    }
    else {
        return imc_avl_insert_rec(vec, data, key);
    }
}








//----------------------------------------------------------------------------//
//-------------------------Memory Management----------------------------------//
//----------------------------------------------------------------------------//

int imc_avl_unref(imc_avl_node_t* tree){
	tree->ref_counter--;

	if(tree->ref_counter == 0){
		imc_avl_unref(tree->left);
		imc_avl_unref(tree->right);
		free(tree);
		return 0;
	}

	return tree->ref_counter;
}
