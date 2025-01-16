#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "markov_chain.h"

int get_random_number(int max_number) {
    return rand() % max_number;
}


char *allocate_and_copy(char *name) {
    char *node_dest = malloc(strlen(name) + 1);
    if (!node_dest) {
        fprintf(stdout, "%s", ALLOCATION_ERROR_MASSAGE);
        return NULL;
    }
    strcpy(node_dest, name);
    return node_dest;
}

/**
 * the function gets random node from the MarkovChain and if we get a word
 * that contains dot at the end of it we retaking another node otherwise
 * returning the chosen Markov_Node.
 * @param markov_chain list of words from the text file
 * @return markov_node without dot at the end of it
 */
MarkovNode *first_node_helper(MarkovChain *markov_chain) {
    int node_number = get_random_number(markov_chain->database->size);
    Node *cur = markov_chain->database->first;
    int i = 0;
    while (i < node_number) {
        cur = cur->next;
        i++;
    }
    unsigned int size_of_word = strlen(cur->data->data);
    if ((strcmp(&cur->data->data[size_of_word - 1], ".")) != 0) {

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
    int node_number = get_random_number(state_struct_ptr->total_frequency);
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
    char sentence[LENGTH_OF_TWEET] = "";
    int num_of_words = 0;
    MarkovNode *cur = first_node;
    char *next_word = NULL;
    while (num_of_words < max_length) {
        next_word = cur->data;
        if (next_word[strlen(next_word) - 1] == '.') {
            strcat(sentence, next_word);
            break;
        }
        strcat(sentence, next_word);
        strcat(sentence, " ");
        num_of_words++;
        cur = get_next_random_node(cur);
    }
    printf("%s", sentence);
}


bool
add_node_to_counter_list(MarkovNode *first_node, MarkovNode *second_node) {
    NextNodeCounter *arr = first_node->counter_list;
    if (!arr) {
        NextNodeCounter *tmp = calloc(1, sizeof(NextNodeCounter));
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
        if (strcmp(first_node->counter_list[i].markovNode->data,
                   second_node->data) == 0) {
            first_node->counter_list[i].frequency += 1;
            first_node->total_frequency += 1;
            return true;
        }
    }
    NextNodeCounter *tmp = realloc(arr, sizeof(NextNodeCounter)
                                        * (first_node->size_of_counter_list +
                                           1));
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


Node *get_node_from_database(MarkovChain *markov_chain, char *data_ptr) {
    if (markov_chain == NULL) {
        return NULL;
    }
    Node *cur = markov_chain->database->first;
    while (cur) {
        if ((strcmp(cur->data->data, data_ptr) != 0)) {
            cur = cur->next;
        } else {
            return cur;
        }
    }
    return NULL;
}

Node *add_to_database(MarkovChain *markov_chain, char *data_ptr) {
    Node *cur = markov_chain->database->first;
    while (cur) {
        if ((strcmp(cur->data->data, data_ptr) == 0)) {
            return cur;
        } else {
            cur = cur->next;
        }
    }
    MarkovNode *new_node = malloc(sizeof(MarkovNode));
    if (!new_node) {
        return NULL;
    }
    new_node->data = allocate_and_copy(data_ptr);
    if (!(new_node->data)) {
        return NULL;
    }

    new_node->counter_list = NULL;
    new_node->size_of_counter_list = 0;
    new_node->total_frequency = 0;
    int add_to = add(markov_chain->database, new_node);
    if (add_to) {
        return NULL;
    }
    return markov_chain->database->last;
}

/**
 * the function free the allocated memory of the Markov_Node
 * @param markov_node
 */
void free_markov_node(MarkovNode *markov_node) {
    free(markov_node->data);
    free(markov_node->counter_list);
    markov_node->counter_list = NULL;
    markov_node->data = NULL;
}

void free_markov_chain(MarkovChain **ptr_chain) {
    MarkovChain *chain = *ptr_chain;
    LinkedList *lst = chain->database;
    Node *cur = lst->first;
    Node *temp;
    while (cur) {
        free_markov_node(cur->data);
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
}
