//
// Created by Miryam on 16/10/2022.
//



#include <string.h>
#include "ProductBST.h"

int main () {
    //test_cmp 1
    /**********Test 1 - build bst**********/
    Node *root = build_bst("presubmit.in");
    if (!root) {
        printf("build_bst returns NULL");
        return 1;
    }


    //test_cmp 4
    /**********Test 2 - add new product to existing tree**********/
    root = add_product(root, "special name", 100);
    if (!search_product(root, "special name")) {
        printf("add_product does not add products to the tree");
        delete_tree(root);
        return 1;
    }


    //test 1
    /**********Test 3 - add product to empty tree**********/
    Node *null_root = add_product(NULL, "something to add", 10);
    if (!null_root) {
        printf("add_product returns NULL after adding to empty tree");
        delete_tree(root);
        return 1;
    }
    delete_tree(null_root);
    delete_tree(root);
    null_root = NULL, root = NULL;


    //test_cmp 8
    /**********Test 4 - delete leaf product**********/
    root = build_bst("presubmit.in_1");
    root = delete_product(root, "Razer Book 13");
    Product *searched = search_product(root, "Razer Book 13");
    if (searched) {
        printf("delete_product does not delete leaf product");
        delete_tree(root);
        return 1;
    }
    delete_tree(root);
    root = NULL, searched = NULL;


    //test_cmp 9
    /**********Test 5 - delete product with one child**********/
    root = build_bst("presubmit.in_2");
    root = delete_product(root, "Lenovo Yoga 9i");
    searched = search_product(root, "Lenovo Yoga 9i");
    if (searched) {
        printf("delete_product does not delete product with one child");
        delete_tree(root);
        return 1;
    }
    delete_tree(root);
    root = NULL, searched = NULL;


    //test_cmp 10
    /**********Test 6 - delete product with two children**********/
    root = build_bst("presubmit.in_3");
    root = delete_product(root, "Dell XPS 13");
    searched = search_product(root, "Dell XPS 13");
    if (searched) {
        printf("delete_product does not delete product with two children");
        delete_tree(root);
        return 1;
    }
    delete_tree(root);
    root = NULL, searched = NULL;


    //test_cmp 12
    /**********Test 7 - search_product**********/
    root = build_bst("presubmit.in");
    searched = search_product(root, "Asus Chromebook Flip CX5");
    if (!searched || strcmp(searched->name, "Asus Chromebook Flip CX5") != 0
        || searched->quantity != 85) {
        printf("search_product does not find an existing product in the tree");
        delete_tree(root);
        return 1;
    }


    //test_cmp 14
    /**********Test 8 - update quantity in positive amount **********/
    root = update_quantity(root, "Asus Zenbook Pro Duo 14", 10);
    searched = search_product(root, "Asus Zenbook Pro Duo 14");
    if (!searched || searched->quantity != 19) {
        printf("update_quantity does not add  the amount to the exist quantity while getting positive input");
        delete_tree(root);
        return 1;
    }
    searched = NULL;


    //test_cmp 15
    /**********Test 9 - update quantity in negative amount **********/
    root = update_quantity(root, "Asus ROG Zephyrus G14", -16);
    searched = search_product(root, "Asus ROG Zephyrus G14");
    if (!searched || searched->quantity != 1) {
        printf("update_quantity does not subtract the amount from the exist quantity while getting negative input");
        delete_tree(root);
        return 1;
    }


    delete_tree(root);
    return 0;

}
//oh_tests "oh_tests/oh_tests.c"
//						ProductBST.c
//						"oh_tests/oh_tests_helpers.c"