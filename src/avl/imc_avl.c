#include "imc_avl.h"
#include <stdlib.h>
#include <stdio.h>

imc_avl_node_t* immutable_right_rotation(imc_avl_node_t* tree){
    if(tree == NULL || tree->left == NULL){
        return NULL;
    }

    imc_avl_node_t* new_right = malloc(sizeof(imc_avl_node_t));
    new_right->ref_counter = 1;
    new_right->data = tree->data;
    new_right->key = tree->key;
    new_right->right = tree->right;
    new_right->balance = 0;
    if(tree->right != NULL){
        tree->right->ref_counter++;
    }
    new_right->left = tree->left->right;
    if(tree->left->right != NULL){
        tree->left->right->ref_counter++;
    }

    imc_avl_node_t* new_root = malloc(sizeof(imc_avl_node_t));
    new_root->ref_counter = 0;
    new_root->data = tree->left->data;
    new_root->key = tree->left->key;
    if(tree->left->left != NULL){
        tree->left->left->ref_counter++;
    }
    new_root->left = tree->left->left;
    new_root->right = new_right;
    new_root->balance = 0;

    return new_root;
}

imc_avl_node_t* immutable_left_rotation(imc_avl_node_t* tree){
    if(tree == NULL || tree->left == NULL){
        return NULL;
    }

    imc_avl_node_t* new_left = malloc(sizeof(imc_avl_node_t));
    new_left->ref_counter = 1;
    new_left->data = tree->data;
    new_left->key = tree->key;
    new_left->left = tree->left;
    new_left->balance = 0;
    if(tree->left != NULL){
        tree->left->ref_counter++;
    }
    new_left->right = tree->right->left;
    if(tree->right->left != NULL){
        tree->right->left->ref_counter++;
    }

    imc_avl_node_t* new_root = malloc(sizeof(imc_avl_node_t));
    new_root->ref_counter = 0;
    new_root->data = tree->right->data;
    new_root->key = tree->right->key;
    new_root->balance=0;
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
int imc_avl_size(imc_avl_node_t* tree){
    imc_avl_node_t* longest_branch = tree;
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
imc_data_t* imc_avl_lookup(imc_avl_node_t* tree, imc_key_t* key,
                           int (*comparator)(imc_key_t*, imc_key_t*)) {
    if (tree != NULL) {
        if (*tree->key == *key) {
            return tree->data;
        } else {
            if (comparator(key, tree->key)) {
                return imc_avl_lookup(tree->right, key,comparator);
            } else {
                return imc_avl_lookup(tree->left, key,comparator);
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
    return temp;
}

// Mutable left rotation
imc_avl_node_t *mutable_left_rotation(imc_avl_node_t* tree){
    if(tree->right == NULL){
        return tree;
    }
    imc_avl_node_t* temp = tree->right;
    tree->right = temp->left;
    temp->left = tree;
    return temp;
}

imc_avl_node_t* imc_avl_insert( imc_avl_node_t* tree,
                                imc_data_t* data, imc_key_t* key,
                                int (*comparator)(imc_key_t*, imc_key_t*),
                                // Data replaced during the insertion
                                imc_data_t** prev_data) {

    imc_avl_node_t* new_node = malloc(sizeof(imc_avl_node_t));
    // we add a new node as a leaf
    if (tree == NULL) {
        new_node->key = key;
        new_node->data = data;
        new_node->balance = 0;
        new_node->ref_counter = 1;
        new_node->left = NULL;
        new_node->right = NULL;
        return new_node;
    }
    // we go through an internal node



    int diff = comparator(key, tree->key);



    if(diff == 0){ // The current node have the same key than the parameter.
        // We duplicate the node and replace the data.
        new_node->key = key;
        new_node->data = data;
        new_node->balance = tree->balance;
        new_node->ref_counter = 1;
        new_node->left = tree->left;
        if(tree->left != NULL) tree->left->ref_counter++;
        new_node->right = tree->right;
        if(tree->right != NULL) tree->right->ref_counter++;
        //We return the previous data.
        *prev_data = tree->data;
        return new_node;

    }


    // we recreate the current node
    new_node->data = tree->data;
    new_node->key = tree->key;
    new_node->ref_counter = 1;


    if (diff > 0) { // We insert in the right branch.
        new_node->right = imc_avl_insert(tree->right, data, key,
                                         comparator, prev_data);
        new_node->left = tree->left;
        if(tree->left != NULL) tree->left->ref_counter++;
    } else { // diff < 0 => We insert in the left branch.
        new_node->left = imc_avl_insert(tree->left, data, key,
                                        comparator, prev_data);
        new_node->right = tree->right;
        if(tree->right != NULL) tree->right->ref_counter++;
    }


    // Now we rebalance the tree.
    if(*prev_data != NULL){ // The structure of the tree wasn't modify.
        new_node->balance = tree->balance;
        return new_node;
    }
    // The structure of the tree was modify.
    if(diff > 0){ // Case where the right branch was modify.
        // The size of the right branch was unchanged.
        if(  tree->right != NULL && (new_node->right->balance == 0  ||
            tree->right->balance == new_node->right->balance)){
            new_node->balance = tree->balance;
        } else {
            // The size of the right branch have increase
            if(tree->balance == 1){ // Rotation is needed
                new_node->balance = 0;
                if(new_node->right->balance == 1){
                new_node->right->balance = 0;
                    new_node = mutable_left_rotation(new_node);
                } else {
                    new_node->right->balance = 0;
                    new_node->right->left->balance = 0;
                    new_node->right = mutable_right_rotation(new_node->right);
                    new_node = mutable_left_rotation(new_node);
                }
            } else { // The tree is still balanced
                new_node->balance = tree->balance +1;
            }
        }
    } else { // diff < 0 => Case where the left branch was modify.
        // The size of the left branch was unchanged.
        if(tree->left != NULL && (new_node->left->balance == 0 ||
            tree->left->balance == new_node->left->balance)){
            new_node->balance = tree->balance;
        } else { // The size of the left branch have increase
            if(tree->balance == -1){ // Rotation is needed
                new_node->balance = 0;
                if(new_node->left->balance == -1){
                new_node->left->balance = 0;
                    new_node = mutable_right_rotation(new_node);
                } else {
                    new_node->left->balance = 0;
                    new_node->left->right->balance = 0;
                    new_node->left = mutable_left_rotation(new_node->left);
                    new_node = mutable_right_rotation(new_node);
                }
            } else { // The tree is still balanced
                new_node->balance = tree->balance -1;
            }
        }
    }

    return new_node;
}

//----------------------------------------------------------------------------//
//-------------------------Remove Functions-----------------------------------//
//----------------------------------------------------------------------------//
imc_avl_node_t* remove_lowest_node(imc_avl_node_t* tree,
                                   imc_avl_node_t** lowest_node){
    if(tree == NULL){
        return NULL;
    }
    //The current node is the lowest
    if(tree->left == NULL){
        *lowest_node = tree;
        return tree->right;
    }
    //The current node isn't the lowest
    imc_avl_node_t* new_left_son = remove_lowest_node(tree->left,lowest_node);
    imc_avl_node_t* new_current = malloc(sizeof(imc_avl_node_t));
    new_current->ref_counter = 0;
    new_current->left = new_left_son;
    if(new_left_son != NULL) new_left_son->ref_counter++;
    new_current->right = tree->right;
    if(new_current->right != NULL) new_current->right->ref_counter++;
    new_current->data = tree->data;
    new_current->key = tree->key;

    //Case where the current node is the parent of the removed one.
    if (new_left_son == NULL) {
        if(new_current->right == NULL)
            new_current->balance = 0;
        else
            new_current->balance = 1;
    //Case where the size of the left branch have changed
    } else if (tree->left->balance != 0 && new_current->left->balance == 0){
        // A rotation is needed
        if(tree->balance == 1){
            new_current->right->ref_counter--;
            // A double left-right rotation
            if(tree->right->balance == -1){
                new_current->balance = 0;
                new_current->right=immutable_right_rotation(new_current->right);
                new_current = mutable_left_rotation(new_current);
            //A simple left rotation
            }else{
                imc_avl_node_t* new_right = malloc(sizeof(imc_avl_node_t));
                new_right->data = new_current->right->data;
                new_right->key = new_current->right->key;
                new_right->ref_counter = 0;
                new_right->balance = 0;
                new_right->right = new_current->right->right;
                if(new_current->right->right!=NULL)
                    new_current->right->right->ref_counter++;
                new_right->left = new_current;
                new_current->ref_counter++;

                new_current->balance = 0;
                if(new_current->right->left!=NULL)
                    new_current->right->left->ref_counter++;
                new_current->right = new_current->right->left;

                new_current = new_right;
            }
        // No rotation needed
        } else {
            new_current->balance = tree->balance+1;
        }
    // Case where the size of the left branch haven't changed
    } else {
        new_current->balance = tree->balance;
    }
    return new_current;
}

imc_avl_node_t* imc_avl_remove( imc_avl_node_t* tree,
                                imc_key_t* key,
                                int (*comparator)(imc_key_t*, imc_key_t*),
                                imc_data_t** removed_data){
    //Base case
    if(tree == NULL){
        return NULL;
    }

    int diff = comparator(key, tree->key);
    imc_avl_node_t* new_node = malloc(sizeof(imc_avl_node_t));
    new_node->ref_counter = 1;
    // The case where the current node is the node
    if(diff == 0){
        imc_avl_node_t* replacement;
        imc_avl_node_t* new_right =remove_lowest_node(tree->right,&replacement);
        new_node->data = replacement->data;
        new_node->key = replacement->key;
        new_node->right = new_right;
        if(new_right != NULL) new_right->ref_counter++;
        new_node->left = tree->left;
        if(tree->left != NULL) tree->left->ref_counter++;
    // Recursives calls
    } else {
        new_node->data = tree->data;
        new_node->key = tree->key;
        if(diff>0){
            imc_avl_node_t* new_right = imc_avl_remove(tree->right,key,
                                                       comparator,removed_data);
            new_node->right = new_right;
            new_node->left = tree->left;
            if(tree->left != NULL) tree->left->ref_counter++;
        } else {
            imc_avl_node_t* new_left = imc_avl_remove(tree->left,key,
                                                      comparator,removed_data);
            new_node->right = new_left;
            new_node->right = tree->right;
            if(tree->right != NULL) tree->right->ref_counter++;
        }
    }

    // The gestion of the balance.
    if(diff < 0){ // the case where we have modify the left branch.
        // if the son's size didn't change then the current node
        // have the same balance than before.
        if(new_node->left->balance != 0 || tree->left->balance == 0){
            new_node->balance = tree->balance;
        } else { // The size of the left branch have changed.
            if(tree->balance != 1){
                new_node->balance = tree->balance + 1;
            } else { // A left rotation is needed.
                new_node->right->ref_counter--;
                if(tree->right->balance >= 0){
                    imc_avl_node_t* new_right = malloc(sizeof(imc_avl_node_t));
                    new_right->data = new_node->right->data;
                    new_right->key = new_node->right->key;
                    new_right->ref_counter = 0;
                    new_right->balance = 0;
                    new_right->right = new_node->right->right;
                    if(new_node->right->right!=NULL)
                        new_node->right->right->ref_counter++;
                    new_right->left = new_node;

                    new_node->balance = 0;
                    if(new_node->right->left!=NULL)
                        new_node->right->left->ref_counter++;
                    new_node->right = new_node->right->left;

                    new_node = new_right;
                // A double left-right rotation is needed.
                } else {
                    new_node->balance = 0;
                    new_node->right = immutable_right_rotation(new_node->right);
                    new_node->right->ref_counter++;
                    new_node = mutable_left_rotation(new_node);
                }
            }
        }
    } else { // the case where we have modify the right branch.
        // if the son's size didn't change then the current node
        // have the same balance than before.
        if(new_node->right->balance != 0 || tree->right->balance == 0){
            new_node->balance = tree->balance;
        } else { // The size of the right branch have changed.
            if(tree->balance != -1){
                new_node->balance = tree->balance - 1;
            } else { // A right rotation is needed.
                new_node->left->ref_counter--;
                if(tree->left->balance <= 0){
                    imc_avl_node_t* new_left = malloc(sizeof(imc_avl_node_t));
                    new_left->data = new_node->left->data;
                    new_left->key = new_node->left->key;
                    new_left->ref_counter = 0;
                    new_left->balance = 0;
                    new_left->left = new_node->left->left;
                    if(new_node->left->left!=NULL)
                        new_node->left->left->ref_counter++;
                    new_left->right = new_node;

                    new_node->balance = 0;
                    if(new_node->left->right!=NULL)
                        new_node->left->right->ref_counter++;
                    new_node->left = new_node->left->right;

                    new_node = new_left;
                // A double right-left rotation is needed.
                } else {
                    new_node->balance = 0;
                    new_node->left = immutable_left_rotation(new_node->left);
                    new_node->left->ref_counter++;
                    new_node = mutable_right_rotation(new_node);
                }
            }
        }
    }
    return new_node;
}

//----------------------------------------------------------------------------//
//-------------------------Dump Function--------------------------------------//
//----------------------------------------------------------------------------//




int _print_t( imc_avl_node_t *tree,
              int is_left,
              int offset,
              int depth,
              char s[20][255],
              void (*print)(imc_key_t*, char* b)) {

    char b[20];
    int width = 5;

    if (!tree) {
        return 0;
    }

    // use the print function from parameter
    print(tree->key, b);
    //sprintf(b, "(%03d)", tree->key);

    int left  = _print_t(tree->left,  1, offset,                depth + 1, s, print);
    int right = _print_t(tree->right, 0, offset + left + width, depth + 1, s, print);

#ifdef COMPACT
    int i;
    for (i = 0; i < width; i++) {
        s[depth][offset + left + i] = b[i];
    }

    if (depth && is_left) {

        for (i = 0; i < width + right; i++) {
            s[depth - 1][offset + left + width/2 + i] = '-';
        }

        s[depth - 1][offset + left + width/2] = '.';

    } else if (depth && !is_left) {

        for (i = 0; i < left + width; i++) {
            s[depth - 1][offset - width/2 + i] = '-';
        }

        s[depth - 1][offset + left + width/2] = '.';
    }
#else
    int i;
    for ( i = 0; i < width; i++) {
        s[2 * depth][offset + left + i] = b[i];
    }

    if (depth && is_left) {

        for ( i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset + left + width + right + width/2] = '+';

    } else if (depth && !is_left) {

        for (i = 0; i < left + width; i++) {
            s[2 * depth - 1][offset - width/2 + i] = '-';
        }

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset - width/2 - 1] = '+';
    }
#endif

    return left + width + right;
}


void imc_avl_dump(imc_avl_node_t* tree,
                void (*print)(imc_key_t*, char* b)) {
    int i;
    char s[20][255];

    printf("*************************************\n");

        for (i = 0; i < 20; i++) {
        sprintf(s[i], "%80s", " ");
    }

    _print_t(tree, 0, 0, 0, s, print);

    for (i = 0; i < 20; i++) {
        printf("%s\n", s[i]);
    }

    printf("*************************************\n");

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

/*int main(){
    int i = 0;
    return 0;
}*/
