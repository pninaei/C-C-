cmake_minimum_required(VERSION 3.24)
project(ex4_pnina_ei)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex4_pnina_ei
        Activation.h
        Dense.h
        main.cpp
        Matrix.h
        MlpNetwork.h Matrix.cpp Activation.cpp Dense.cpp MlpNetwork.cpp)
