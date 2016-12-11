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
    new_root-left = new_left;

    return new_root;
}
