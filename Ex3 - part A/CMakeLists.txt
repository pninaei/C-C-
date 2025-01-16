cmake_minimum_required(VERSION 3.24)
project(ex3a_pnina_ei C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(ex3a_pnina_ei markov_chain.h tweets_generator.c
        markov_chain.c linked_list.c linked_list.h tests.c tests.h tweet_test.txt)
