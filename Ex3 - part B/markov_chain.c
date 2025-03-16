#include "markov_chain.h"
#include <stdlib.h>
#include <stdio.h>

int get_random_number(int max_number) {
    return rand() % max_number;
}
MarkovNode *first_node_helper(MarkovChain *markov_chain) {
    int node_number = get_random_number
            (markov_chain->database->size);
    Node *cur = markov_chain->database->first;
    int i = 0;
    while (i < node_number) {
        cur = cur->next;
        i++;
    }
    if ((markov_chain->is_last((cur->data->data)) == 0)) {
        return cur->data;
    }
    return NULL;
}
MarkovNode *get_first_random_node(MarkovChain *markov_chain) {
    MarkovNode *cur = first_node_helper(markov_chain);
    while (cur == NULL) {
        cur = first_node_helper(markov_chain);
    }
    return cur;
}

MarkovNode *get_next_random_node(MarkovNode *state_struct_ptr) {
    int node_number = get_random_number
            (state_struct_ptr->total_frequency);
    NextNodeCounter *arr = state_struct_ptr->counter_list;
    int index = 0;
    while (index < (node_number)) {
        if (node_number - arr->frequency >= index) {
            index += arr->frequency;
            arr++;
        } else {
            return arr->markovNode;
        }
    }
    return arr->markovNode;
}


void generate_random_sequence(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length) {
    if (!first_node) {
        first_node = get_first_random_node(markov_chain);
    }
    int num_of_words = 0;
    MarkovNode *cur = first_node;
    while (num_of_words < max_length) {
        if (markov_chain->is_last(cur->data) == 1) {
            markov_chain->print_func(cur->data);
            break;
        }
        markov_chain->print_func(cur->data);
        if (num_of_words == max_length-1){
            break;
        }
        cur = get_next_random_node(cur);
        num_of_words++;
    }
}

bool add_node_to_counter_list(MarkovNode *first_node, MarkovNode
*second_node, MarkovChain *markov_chain){
    NextNodeCounter *arr = first_node->counter_list;
    if (!arr) {
        NextNodeCounter *tmp = calloc(1,
                                      sizeof(NextNodeCounter));
        if (!tmp) {
            return false;
        }
        first_node->counter_list = tmp;
        first_node->counter_list->markovNode = second_node;
        first_node->counter_list->frequency = 1;
        first_node->size_of_counter_list = 1;
        first_node->total_frequency = 1;
        return true;
    }
    for (int i = 0; i < first_node->size_of_counter_list; i++) {
        if (markov_chain->comp_func
        (first_node->counter_list[i].markovNode->data,second_node->data) == 0){
            first_node->counter_list[i].frequency += 1;
            first_node->total_frequency += 1;
            return true;
        }
    }
    NextNodeCounter *tmp = realloc(arr, sizeof(NextNodeCounter)
    * (first_node->size_of_counter_list +1));
    if (!tmp) {
        return false;
    }
    first_node->counter_list = tmp;
    first_node->counter_list[first_node->size_of_counter_list]
            .markovNode = second_node;
    first_node->counter_list[first_node->size_of_counter_list].frequency = 1;
    first_node->size_of_counter_list += 1;
    first_node->total_frequency += 1;
    return true;
}

Node *get_node_from_database(MarkovChain *markov_chain, void *data_ptr){
    if (markov_chain == NULL) {
        return NULL;
    }
    Node *cur = markov_chain->database->first;
    while (cur) {
        if ((markov_chain->comp_func(cur->data->data, data_ptr) != 0)) {
            cur = cur->next;
        } else {
            return cur;
        }
    }
    return NULL;
}


Node *add_to_database(MarkovChain *markov_chain, void *data_ptr) {
    Node *cur = markov_chain->database->first;
    while (cur) {
        if (markov_chain->comp_func(cur->data->data, data_ptr) == 0) {
            return cur;
        } else {
            cur = cur->next;
        }
    }
    MarkovNode *new_node = malloc(sizeof(MarkovNode));
    if (!new_node) {
        return NULL;
    }
    new_node->data = markov_chain->copy_func(data_ptr);
    if (!(new_node->data)) {
        free(new_node);
        new_node = NULL;
        return NULL;
    }

    new_node->counter_list = NULL;
    new_node->size_of_counter_list = 0;
    new_node->total_frequency = 0;
    int add_to = add(markov_chain->database, new_node);
    if (add_to) {
        markov_chain->free_data(new_node->data);
        new_node->data = NULL;
        free(new_node);
        new_node = NULL;
        return NULL;
    }
    return markov_chain->database->last;
}


void free_markov_chain(MarkovChain **ptr_chain) {
    MarkovChain *chain = *ptr_chain;
    LinkedList *lst = chain->database;
    Node *cur = lst->first;
    Node *temp;
    while (cur) {
        if (cur->data->counter_list != NULL){
            free(cur->data->counter_list);
            cur->data->counter_list = NULL;
        }
        chain->free_data(cur->data->data);
        cur->data->data = NULL;
        free(cur->data);
        cur->data = NULL;
        temp = cur;
        cur = cur->next;
        free(temp);
    }
    cur = NULL;
    free(lst);
    lst = NULL;
    free(chain);
    chain = NULL;
    *ptr_chain = NULL;
}
