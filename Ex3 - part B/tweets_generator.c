#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "markov_chain.h"
#include "linked_list.h"

#define MAX_ARGS 5
#define OPTIONAL_ARGS 4
#define MAX_PER_TWEET 20
#define INVALID_FILE "Error: can't open the file"
#define INVALID_NUM_OF_ARGS "Usage: required seed, number of tweets, path to"\
 "file and number of words to read (optional)\n"

/**
 * the function compare between 2 string
 * @param str1
 * @param str2
 * @return 0 if the equal, positive number if the first is bigger and negative
 * otherwise
 */
static int compare_strings(const void *str1, const void *str2) {
    char *str_1 = (char *) str1;
    char *str_2 = (char *) str2;
    return strcmp(str_1, str_2);
}

/**
 * the function free the allocated memory
 * @param node_data
 */
static void free_data_node(void *node_data) {
    char *data = (char *) node_data;
    free(data);
    data = NULL;
}

/**
 * the function allocated memory to new string and return a pointer to that
 * string
 * @param name
 * @return NULL in case of failure allocation and the pointer otherwise
 */
static void *allocate_and_copy(void *name) {
    char *str_name = (char *) name;
    char *node_dest = malloc(strlen(str_name) + 1);
    if (!node_dest) {
        fprintf(stdout, "%s", ALLOCATION_ERROR_MASSAGE);
        return NULL;
    }
    strcpy(node_dest, str_name);
    return (void *) node_dest;
}

/**
 * the function checks if the last char of the string is a dot if so return
 * true and false otherwise
 * @param str
 * @return
 */
static bool is_last(void *str) {
    char *string = (char *) str;
    unsigned int size_of_word = strlen(string);
    if (string[size_of_word - 1] == '.') {
        return true;
    } else {
        return false;
    }
}

/**
 * the function prints tweets
 * @param data - word to print
 */
static void print_data_char(const void *data) {
    char *str = (char *) data;
    if ((is_last((void *) str) == 1)) {
        printf("%s", str);
    } else {
        printf("%s ", str);
    }
}


static int fill_database(FILE *fp, int words_to_read, MarkovChain
*markov_chain) {
    char buffer[LENGTH_OF_TWEET];
    char *next_word = NULL;
    char *word = NULL;
    Node *first = NULL;
    Node *sec = NULL;
    while (fgets(buffer, LENGTH_OF_TWEET, fp) && words_to_read) {
        if (strcmp(buffer, "\n") == 0) {
            continue;
        }
        char *tmp = NULL;
        word = strtok(buffer, " \r\n");
        if (word == NULL){
            break;
        }
        first = add_to_database(markov_chain, (void *) word);
        if (!first) {
            return EXIT_FAILURE;
        }
        words_to_read -= 1;
        next_word = strtok(NULL, " \r\n");
        while (next_word && words_to_read) {
            if (!(get_node_from_database(markov_chain, (void *) next_word))) {
                sec = add_to_database(markov_chain, (void *) next_word);
                if (!sec) {
                    return EXIT_FAILURE;
                }
            } else {
                sec = get_node_from_database(markov_chain, (void *) next_word);
            }
            if (tmp) {
                if (tmp[strlen(tmp) - 1] != '.') {
                    bool add_to = add_node_to_counter_list
                            (first->data, sec->data, markov_chain);
                    if (!add_to) {
                        return EXIT_FAILURE;
                    }
                    tmp = next_word;
                }
                else{
                    tmp = next_word;
                }
            } else {
                tmp = next_word;
                bool add_to = add_node_to_counter_list
                        (first->data, sec->data, markov_chain);
                if (!add_to) {
                    return EXIT_FAILURE;
                }
            }
            words_to_read -= 1;
            next_word = strtok(NULL, " \r\n");
            if (next_word == NULL) {
                break;
            }
            first = sec;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * the function prints the tweet to the user line by line according to the
 * number of the tweet
 * @param markov_chain the database
 * @param num_of_tweets number of tweets to generate
 */
static void making_tweets(MarkovChain *markov_chain, int num_of_tweets) {
    for (int i = 0; i < num_of_tweets; i++) {
        printf("Tweet %d: ", i + 1);
        MarkovNode *first_node = get_first_random_node(markov_chain);
        generate_random_sequence(markov_chain, first_node,MAX_PER_TWEET);
        printf("\n");

    }
}

int main(int argc, char *argv[]) {
    if (argc < OPTIONAL_ARGS || argc > MAX_ARGS) {
        fprintf(stdout, "%s", INVALID_NUM_OF_ARGS);
        return EXIT_FAILURE;
    }
    unsigned int seed = strtol(argv[1], NULL, 10);
    srand(seed);
    long num_of_tweets = strtol(argv[2], NULL, 10);
    FILE *file = fopen(argv[3], "r");
    if (!file) {
        fprintf(stdout, "%s", INVALID_FILE);
        return EXIT_FAILURE;
    }
    LinkedList *lst = calloc(1, sizeof(LinkedList));
    if (!lst) {
        fprintf(stdout, "%s", ALLOCATION_ERROR_MASSAGE);
        return EXIT_FAILURE;
    }
    MarkovChain *markov_chain = malloc(sizeof(MarkovChain));
    if (!markov_chain) {
        free(lst);
        fprintf(stdout, "%s", ALLOCATION_ERROR_MASSAGE);
        return EXIT_FAILURE;
    }
    markov_chain->database = lst;
    markov_chain->print_func = print_data_char;
    markov_chain->comp_func = compare_strings;
    markov_chain->free_data = free_data_node;
    markov_chain->copy_func = allocate_and_copy;
    markov_chain->is_last = is_last;
    if (argc == OPTIONAL_ARGS) {
        int fill = fill_database(file, -1, markov_chain);
        if (fill == 1) {
            fprintf(stdout, "%s", ALLOCATION_ERROR_MASSAGE);
            fclose(file);
            free_markov_chain(&markov_chain);
            return EXIT_FAILURE;
        }
    } else {
        unsigned int words_to_read = strtol(argv[4], NULL, 10);
        int fill = fill_database(file, words_to_read, markov_chain);
        if (fill == 1) {
            fprintf(stdout, "%s", ALLOCATION_ERROR_MASSAGE);
            fclose(file);
            free_markov_chain(&markov_chain);
            return EXIT_FAILURE;
        }
    }
    fclose(file);
    making_tweets(markov_chain, num_of_tweets);
    free_markov_chain(&markov_chain);
    return EXIT_SUCCESS;
}