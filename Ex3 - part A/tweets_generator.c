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
 * the function removes \n from words that has them so that we be able to
 * get them into the database without the character
 * @param word
 * @return fix word without the character \n in it
 */
char *removing_n(char *word){
    char *fix = word;
    if (fix[strlen(word)-1] == '\n'){
        fix[strlen(word)-1] = '\0';
        return fix;
    }
    return word;
}

int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain) {
    char buffer[LENGTH_OF_TWEET];
    char *next_word = NULL;
    char *word = NULL;
    Node *first = NULL;
    Node *sec = NULL;
    while (fgets(buffer, LENGTH_OF_TWEET, fp) && words_to_read) {
        if (strcmp(buffer, "\n") == 0){
            continue;
        }
        char *tmp = NULL;
        word = strtok(buffer, " ");
        first = add_to_database(markov_chain, word);
        if (!first) {
            return EXIT_FAILURE;
        }
        words_to_read -=1;
        next_word = strtok(NULL, " ");
        next_word = removing_n(next_word);
        while (next_word && words_to_read) {
            if (!(get_node_from_database(markov_chain, next_word))) {
                sec = add_to_database(markov_chain, next_word);
                if (!sec) {
                    return EXIT_FAILURE;
                }
            }
            else{
                sec = get_node_from_database(markov_chain, next_word);
            }
            if (tmp) {
                if (tmp[strlen(tmp) - 1] != '.') {
                    bool add_to = add_node_to_counter_list
                            (first->data, sec->data);
                    if (!add_to) {
                        return EXIT_FAILURE;
                    }
                    tmp = next_word;
                } else {
                    tmp = next_word;
                }
            } else {
                tmp = next_word;
                bool add_to = add_node_to_counter_list(first->data, sec->data);
                if (!add_to) {
                    return EXIT_FAILURE;
                }
            }
            words_to_read -=1;
            next_word = strtok(NULL, " ");
            if (next_word == NULL){
                break;
            }
            next_word = removing_n(next_word);
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
void making_tweets(MarkovChain *markov_chain, int num_of_tweets) {
    for (int i = 0; i < num_of_tweets; i++) {
        printf("Tweet %d: ", i + 1);
        MarkovNode *first_node = get_first_random_node(markov_chain);
        generate_random_sequence(markov_chain, first_node, MAX_PER_TWEET);
        printf("\n");

    }
}
int main(int argc, char *argv[]) {
    if (argc < OPTIONAL_ARGS || argc > MAX_ARGS) {
        fprintf(stdout, "%s", INVALID_NUM_OF_ARGS);
        return EXIT_FAILURE;
    }
    long seed = strtol(argv[1], NULL, 10);
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
    if (argc == OPTIONAL_ARGS) {
        int fill = fill_database(file, -1, markov_chain);
        if (fill == 1){
            fclose(file);
            free_markov_chain(&markov_chain);
            return EXIT_FAILURE;
        }
    } else {
        long words_to_read = strtol(argv[4], NULL, 10);
        int fill = fill_database(file, words_to_read, markov_chain);
        if (fill == 1) {
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