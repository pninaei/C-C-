#include <iostream>
#include "Dense.h"
#define INVALID_SIZES "the matrices are not the same size"
#define NUM_OF_COL_FOR_BIAS 1
Dense::Dense(const Matrix &weights, const Matrix &bias, active_func func)
{
    if (bias.get_cols() != NUM_OF_COL_FOR_BIAS){
        throw std::domain_error(INVALID_SIZES);
    }
    if (weights.get_rows() != bias.get_rows()){
        throw std::domain_error(INVALID_SIZES);
    }
    _weights = weights;
    _bias = bias;
    _func = func;
}

const Matrix& Dense::get_weights() const { return this->_weights;}

const Matrix& Dense::get_bias() const { return this->_bias;}

const active_func& Dense::get_activation() const { return this->_func;}

Matrix Dense::operator()(const Matrix &vector) const{
    if (get_weights().get_cols() != vector.get_rows()){
        throw std::domain_error(INVALID_SIZES);
    }
    Matrix output = get_activation()((get_weights() * vector) + get_bias());
    return output;
}



