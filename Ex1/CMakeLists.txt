cmake_minimum_required(VERSION 3.24)
project(ex1_pnina_ei C)

set(CMAKE_C_STANDARD 99)

add_executable(ex1_pnina_ei main.c cipher.c tests.c tests.h cipher.h testing.txt output_testing.txt ../ex2-pnina_ei/presubmit.c ../ex2-pnina_ei/presubmit.c)
