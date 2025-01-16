#include "cipher.h"
#include "tests.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE 1025
#define ARG_FOR_TEST 2
#define ARG_FOR_CODE 5
#define NUM_OF_TESTS 10
#define STRTDIGIT 48
#define LSTDIGIT 57

int run_tests(void) {
    // the function runs the tests and checks if they all passed. if so,
    // return EXIT_SUCCESS otherwise, return EXIT_FAILURE.

    int (*tests[])() = {
            test_encode_non_cyclic_lower_case_positive_k,
            test_encode_cyclic_lower_case_special_char_positive_k,
            test_encode_non_cyclic_lower_case_special_char_negative_k,
            test_encode_cyclic_lower_case_negative_k,
            test_encode_cyclic_upper_case_positive_k,
            test_decode_non_cyclic_lower_case_positive_k,
            test_decode_cyclic_lower_case_special_char_positive_k,
            test_decode_non_cyclic_lower_case_special_char_negative_k,
            test_decode_cyclic_lower_case_negative_k,
            test_decode_cyclic_upper_case_positive_k};

    for (int i = 0; i < NUM_OF_TESTS; i++) {
        if (tests[i]() == 1) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int encode_decode(char *command, char *input_file, char *output_file, int
shift) {
    // the function encoding / decoding and checks the validity of the files
    FILE *file = fopen(input_file, "r");
    if (file == NULL) {
        fprintf(stderr, "The given file is invalid.\n");
        return EXIT_FAILURE;
    }
    FILE *out_file = fopen(output_file, "w");
    if (out_file == NULL) {
        fprintf(stderr, "The given file is invalid.\n");
        return EXIT_FAILURE;
    }
    char line[MAX_LINE] = {0}; // empty line - the text will be here
    if (strcmp(command, "encode") == 0) {
        while (fgets(line, MAX_LINE, file)) {
            encode(line, shift);
            fputs(line, out_file);
        }
        fclose(file);
        fclose(out_file);
        return EXIT_SUCCESS;
    } else {
        while (fgets(line, MAX_LINE, file)) {
            decode(line, shift);
            fputs(line, out_file);
        }
        fclose(file);
        fclose(out_file);
        return EXIT_SUCCESS;
    }
}

int main(int argc, char *argv[]) {
    if (argc != ARG_FOR_CODE && argc != ARG_FOR_TEST) {
        fprintf(stderr, "The program receives 1 or 4 arguments only.\n");
        return EXIT_FAILURE;
    }
    // if the single arg that we got is the word "test"
    if (argc == 2) {
        if (strcmp(argv[1], "test") != 0) {
            fprintf(stderr, "Usage: cipher test\n");
            return EXIT_FAILURE;
        }
        return run_tests();
    }
    if ((strcmp(argv[1], "encode") != 0) && (strcmp(argv[1], "decode") != 0)) {
        fprintf(stderr, "The given command is invalid.\n");
        return EXIT_FAILURE;
    }
    char *k = argv[2];
    // if k is only minus sign
    if (k[0] == '-') {
        if (k[1] == '\0') {
            fprintf(stderr, "The given shift value is invalid.\n");
            return EXIT_FAILURE;
        }
        for (int i = 1; k[i] != '\0'; i++){
            if (STRTDIGIT > k[i] || k[i] > LSTDIGIT) {
                fprintf(stderr, "The given shift value is invalid.\n");
                return EXIT_FAILURE;
            }
        }
    }
    else {
        for (int i = 0; k[i] != '\0'; i++){
            if (STRTDIGIT > k[i] || k[i] > LSTDIGIT) {
                fprintf(stderr, "The given shift value is invalid.\n");
                return EXIT_FAILURE;
            }
        }
    }
    long key = strtol(argv[2], NULL, 10);
    return encode_decode(argv[1], argv[3], argv[4], key);
}
