#include "ProductBST.h"
#include <string.h>
#ifdef OH_TESTS
#include "oh_tests/oh_tests_helpers.h"
#define malloc test_malloc
#define calloc test_calloc
#define free test_free
#define stderr get_test_stderr()
#endif // OH_TESTS

/**
 * the function free the memory of the given node
 * @param node pointer to a node from the tree
 */
void free_node(Node *node) {
    //printf("deleting node : %s", node->product.name);
    free(node->product.name);
    free(node);
    node = NULL;
}

void delete_tree(Node *root) {
    if (root) {
        delete_tree(root->left_child);
        delete_tree(root->right_child);
        free_node(root);
    }
}

/**
 * the function allocate memory space and copy the string from name to dest
 * @param name - string to copy to node_dest
 * @return - the string
 */
char *allocate_and_copy(char *name) {

    char *node_dest = malloc(strlen(name) + 1);
    if (!node_dest) {
        fprintf(stderr, "%s%s", ERROR, ALLOCATION_FAILED);
        return NULL;
    }
    strcpy(node_dest, name);
    return node_dest;
}

/**
 * the function creates a new node with the struct Products filled
 * @param name - the name of the product
 * @param quantity the quantity of the product
 * @return pointer to the new node
 */
Node *new_node(char *name, int quantity) {
    Node *node = malloc(sizeof(Node));
    if (!node) {
        fprintf(stderr, "%s%s", ERROR, ALLOCATION_FAILED);
        return NULL;
    }
    if (!(node->product.name = allocate_and_copy(name))) {
        free(node);
        node = NULL;
        return NULL;
    }
    node->product.quantity = quantity;
    node->right_child = NULL;
    node->left_child = NULL;

    return node;
}

/**
 * the function search in the BST if the product exist and if not put it in
 * otherwise return the original root without any changes
 * @param root - pointer to the root of the BST
 * @param name - name of product to put in the BST
 * @param quantity - represent the amount of the product
 * @return pointer to the root of the BST
 */

Node *search_in_tree(Node *root, char *name, int quantity) {
    if (root->left_child == NULL && root->right_child == NULL) {
        if (strcmp(root->product.name, name) == 0) {
            fprintf(stderr, "%s%s", ERROR, PRODUCT_EXISTS);
        } else { // the product isn't in the BST
            if (strcmp(root->product.name, name) < 0) {
                if (!(root->right_child = new_node(name, quantity))) {
                    return NULL;
                }
            } else {
                if (!(root->left_child = new_node(name, quantity))) {
                    return NULL;
                }
            }
        }
    } else if (root->left_child == NULL) {
        if (strcmp(root->product.name, name) == 0) {
            fprintf(stderr, "%s%s", ERROR, PRODUCT_EXISTS);
        } else if (strcmp(root->product.name, name) < 0) {
            return search_in_tree(root->right_child, name, quantity);
        } else {
            if (!(root->left_child = new_node(name, quantity))) {
                return NULL;
            }
        }
    } else if (root->right_child == NULL) {
        if (strcmp(root->product.name, name) == 0) {
            fprintf(stderr, "%s%s", ERROR, PRODUCT_EXISTS);
        } else if (strcmp(root->product.name, name) < 0) {
            if (!(root->right_child = new_node(name, quantity))) {
                return NULL;
            }
        } else {
            return search_in_tree(root->left_child, name, quantity);
        }
    } else {
        if (strcmp(root->product.name, name) == 0) {
            fprintf(stderr, "%s%s", ERROR, PRODUCT_EXISTS);
        } else if (strcmp(root->product.name, name) < 0) {
            return search_in_tree(root->right_child, name, quantity);
        } else {
            return search_in_tree(root->left_child, name, quantity);
        }
    }
    return root;
}

Node *add_product(Node *root, char *name, int quantity) {
    if (quantity <= 0) { // if the quantity is invalid
        fprintf(stderr, "%s%s", ERROR, INVALID_QUANTITY);
        return root;
    }
    if (name == NULL) { // the name is invalid
        fprintf(stderr, "%s%s", ERROR, INVALID_POINTER);
        return root;
    }
    if (!root) { // if there is no root
        root = new_node(name, quantity);
        if (root == NULL) {
            delete_tree(root);
            return NULL;
        }
        return root;
    } else {

        if (!search_in_tree(root, name, quantity)) { // if the allocation of
            // memory failed
            delete_tree(root);
            return NULL;
        } else {
            return root;
        }
    }
}

Node *build_bst(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "%s%s", ERROR, FILE_OPEN_FAILED);
        return NULL;
    }
    char line[MAX_LINE_LENGTH] = {0}; //empty line
    Node *node = NULL;
    Product product;
    char *tok = NULL;
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        if (!(tok = strtok(line, ":"))) {
            return NULL;
        }
        product.name = tok;
        char *ptr;
        int amount = strtol(strtok(NULL, ":"), &ptr, 10);
        if (!ptr || (*ptr) == '\n' || (*ptr) == '\r') {
            product.quantity = amount;
            node = add_product(node, product.name, product.quantity);
            if (!node) {
                return NULL;
            }
        } else {
            fprintf(stderr, "%s%s", ERROR, INVALID_LINE);
        }
    }
    fclose(file);
    return node;
}

/**
 * the function return the minimum product in the tree
 * @param root - pointer to the root of the BST
 * @return return pointer to the minimum element
 */

Node *finding_successor(Node *root) {
    // the successor is the left most in the right subtree
    if (root == NULL) {
        return NULL;
    }
    Node *tmp = root;
    while (tmp && tmp->left_child != NULL) {
        tmp = tmp->left_child;
    }
    return tmp;

}

Node *delete_product(Node *root, char *name) {
    if (name == NULL) {
        fprintf(stderr, "%s%s", ERROR, INVALID_POINTER);
        return root;
    }
    if (root == NULL) {
        fprintf(stderr, "%s%s", ERROR, PRODUCT_NOT_FOUND);
        return NULL;
    }
    // getting into the right subtree
    if ((strcmp(root->product.name, name) < 0)) {
        root->right_child = delete_product(root->right_child, name);
        // getting into the left subtree
    } else if ((strcmp(root->product.name, name) > 0)) {
        root->left_child = delete_product(root->left_child, name);
    }
        // we reached to the end of the tree - to a leaf
    else {
        if (root->left_child == NULL && root->right_child == NULL) {
            // checking if the product exists
            if ((strcmp(root->product.name, name) != 0)) {
                fprintf(stderr, "%s%s", ERROR, PRODUCT_NOT_FOUND);
            } else {
                free(root->product.name);
                free(root);
                root = NULL;
                //return NULL;
            }
        }
            // have a right child only
        else if (root->right_child == NULL) {
            Node *tmp = root->left_child;
            free(root->product.name);
            free(root);
            return tmp;
            // have a left child only
        } else if (root->left_child == NULL) {
            Node *tmp = root->right_child;
            free(root->product.name);
            free(root);
            return tmp;
        }
            // have 2 children
        else {
            Node *successor = finding_successor(root->right_child);
            free(root->product.name);
            if (!(root->product.name = allocate_and_copy(successor->product
                    .name))){
                return NULL;
            }
            else{
                root->product.quantity = successor->product.quantity;
                root->right_child = delete_product(root->right_child,
                                                   successor->product.name);
            }
        }
    }
    return root;
}

Product *search_product(Node *root, char *name) {
    if (root == NULL) {
        return NULL;
    }
    if (name == NULL) {
        fprintf(stderr, "%s%s", ERROR, INVALID_POINTER);
        return NULL;
    }
    if (root->right_child == NULL && root->left_child == NULL) {
        if ((strcmp(root->product.name, name) != 0)) {
            //fprintf(stderr, "%s%s", ERROR, PRODUCT_NOT_FOUND);
            return NULL;
        } else {
            return &(root->product);
        }
    }
    if ((strcmp(root->product.name, name) > 0)) {
        return search_product(root->left_child, name);
    } else if ((strcmp(root->product.name, name) < 0)) {
        return search_product(root->right_child, name);
    } else {
        return &(root->product);
    }
}


Node *update_quantity(Node *root, char *name, int amount_to_update) {
    if (name == NULL) {
        fprintf(stderr, "%s%s", ERROR, INVALID_POINTER);
        return root;
    }
    if (root == NULL) {
        fprintf(stderr, "%s%s", ERROR, INVALID_QUANTITY);
        return root;
    }
    if ((strcmp(root->product.name, name) < 0)) {
        root->right_child = update_quantity(root->right_child, name,
                                            amount_to_update);
    } else if ((strcmp(root->product.name, name) > 0)) {
        root->left_child = update_quantity(root->left_child, name,
                                           amount_to_update);
    } else {
        if ((strcmp(root->product.name, name) != 0)) {
            fprintf(stderr, "%s%s", ERROR, PRODUCT_NOT_FOUND);
        } else {
            if (amount_to_update < 0) {
                int new_amount = root->product.quantity - abs
                        (amount_to_update);
                if (new_amount < 0) {
                    fprintf(stderr, "%s%s", ERROR, INVALID_QUANTITY);
                } else if (new_amount > 0) {
                    root->product.quantity = new_amount;
                } else {
                    Node *update = delete_product(root, root->product.name);
                    return update;
                }
            }
            if (amount_to_update > 0) {
                root->product.quantity += amount_to_update;
            }
        }
    }
    return root;
}