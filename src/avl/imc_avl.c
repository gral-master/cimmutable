#include "imc_avl.h"
#include <stdlib.h>

// TODO : manage the balance attribute
imc_avl_node_t* right_rotation(imc_avl_node_t* tree){
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
imc_avl_node_t* left_rotation(imc_avl_node_t* tree){
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

//----------------------------------------------------------------------------//
//---------------------------Size Functions-----------------------------------//
//----------------------------------------------------------------------------//
int imc_avl_size(imc_avl_node_t* vec){
    imc_avl_node_t* longest_branch = vec;
    int size=0;
    while(longest_branch != NULL){
        size++;
        if(longest_branch->balance==1)
            longest_branch = longest_branch->right;
        else
            longest_branch = longest_branch->left;
    }
    return size;
}

//----------------------------------------------------------------------------//
//-------------------------Lookup Functions-----------------------------------//
//----------------------------------------------------------------------------//
imc_data_t* imc_avl_lookup(imc_avl_node_t* vec, imc_key_t* key,
                           int (*comparator)(imc_key_t*, imc_key_t*)) {
    if (vec != NULL) {
        if (*vec->key == *key) {
            return vec->data;
        } else {
            if (comparator(key, vec->key)) {
                return imc_avl_lookup(vec->right, key,comparator);
            } else {
                return imc_avl_lookup(vec->left, key,comparator);
            }
        }
    } else {
        return NULL;
    }
}

//----------------------------------------------------------------------------//
//-------------------------Insert Functions-----------------------------------//
//----------------------------------------------------------------------------//

// Mutable right rotation
imc_avl_node_t *mutable_right_rotation(imc_avl_node_t* tree){
    if(tree->left == NULL){
        return tree;
    }
    imc_avl_node_t* temp = tree->left;
    tree->left = temp->right;
    temp->right = tree;
    return tree;
}

// Mutable left rotation
imc_avl_node_t *mutable_left_rotation(imc_avl_node_t* tree){
    if(tree->left == NULL){
        return tree;
    }
    imc_avl_node_t* temp = tree->right;
    tree->right = temp->left;
    temp->left = tree;
    return tree;
}

imc_avl_node_t* imc_avl_insert( imc_avl_node_t* vec,
                                imc_data_t* data, imc_key_t* key,
                                int (*comparator)(imc_key_t*, imc_key_t*),
                                // Data replaced during the insertion
                                imc_data_t** prev_data) {

    imc_avl_node_t* new_node = malloc(sizeof(imc_avl_node_t));
    // we add a new node as a leaf
    if (vec == NULL) {
        new_node->key = key;
        new_node->data = data;
        new_node->balance = 0;
        new_node->ref_counter = 1;
        new_node->left = NULL;
        new_node->right = NULL;
        return new_node;
    }
    // we go through an internal node
    int diff = comparator(key, vec->key);

    if(diff == 0){ // The current node have the same key than the parameter.
        // We duplicate the node and replace the data.
        new_node->key = key;
        new_node->data = data;
        new_node->balance = vec->balance;
        new_node->ref_counter = 1;
        new_node->left = vec->left;
        if(vec->left != NULL) vec->left->ref_counter++;
        new_node->right = vec->right;
        if(vec->right != NULL) vec->right->ref_counter++;
        //We return the previous data.
        *prev_data = vec->data;
        return new_node;

    }
    // we recreate the current node
    new_node->data = vec->data;
    new_node->key = vec->key;
    new_node->ref_counter = 1;

    if (diff > 0) { // We insert in the right branch.
        new_node->right = imc_avl_insert(vec->right, data, key,
                                         comparator, prev_data);
        new_node->left = vec->left;
        if(vec->left != NULL) vec->left->ref_counter++;
    } else { // diff < 0 => We insert in the left branch.
        new_node->left = imc_avl_insert(vec->left, data, key,
                                        comparator, prev_data);
        new_node->right = vec->right;
        if(vec->right != NULL) vec->right->ref_counter++;
    }

    // Now we rebalance the tree.
    if(*prev_data != NULL){ // The structure of the tree wasn't modify.
        new_node->balance = vec->balance;
        return new_node;
    }
    // The structure of the tree was modify.
    if(diff > 0){ // Case where the right branch was modify.
        // The size of the right branch was unchanged.
        if(vec->right->balance == new_node->right->balance
          || new_node->right->balance == 0){
            new_node->balance = vec->balance;
        } else { // The size of the right branch have increase
            if(vec->balance == 1){ // Rotation is needed
                new_node->balance = 0;
                new_node->right->balance = 0;
                if(new_node->right->balance == 1){
                    new_node = mutable_left_rotation(new_node);
                } else {
                    new_node->right->left->balance = 0;
                    new_node->right = mutable_right_rotation(new_node->right);
                    new_node = mutable_left_rotation(new_node);
                }
            } else { // The tree is still balanced
                new_node->balance = vec->balance +1;
            }
        }
    } else { // diff < 0 => Case where the left branch was modify.
        // The size of the left branch was unchanged.
        if(vec->left->balance == new_node->left->balance
          || new_node->left->balance == 0){
            new_node->balance = vec->balance;
        } else { // The size of the left branch have increase
            if(vec->balance == -1){ // Rotation is needed
                new_node->balance = 0;
                new_node->left->balance = 0;
                if(new_node->left->balance == -1){
                    new_node = mutable_right_rotation(new_node);
                } else {
                    new_node->left->right->balance = 0;
                    new_node->left = mutable_left_rotation(new_node->left);
                    new_node = mutable_right_rotation(new_node);
                }
            } else { // The tree is still balanced
                new_node->balance = vec->balance -1;
            }
        }
    }

    return new_node;
}

//----------------------------------------------------------------------------//
//-------------------------Memory Management----------------------------------//
//----------------------------------------------------------------------------//

int imc_avl_unref(imc_avl_node_t* tree){
	tree->ref_counter--;
//TODO not thread safe
    if(tree->ref_counter <= 0){
        imc_avl_unref(tree->left);
        imc_avl_unref(tree->right);
        free(tree);
        return 0;
    }

    return tree->ref_counter;
}

//***********************Test part FUNCTION***********************************//
int is_sup(imc_key_t* x, imc_key_t* y)
{
    return *x > *y;
}

int main(){
    int i = 0;
    return 0;
}
