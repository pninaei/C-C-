#include "Activation.h"
#include "cmath"
Matrix activation::relu(const Matrix &other)
{
    Matrix new_mat(other.get_rows(), other.get_cols());
    for (int i=0; i < other.get_rows() * other.get_cols(); i++){
        if (other[i] >= 0){
            new_mat[i] = other[i];
        } else{
            new_mat[i] = 0;
        }
    }
    return new_mat;
}

Matrix activation::softmax(const Matrix &other)
{
    Matrix new_mat(other.get_rows(), other.get_cols());
    float sum = 0;
    for (int i=0; i < other.get_rows() * other.get_cols(); i++){
        new_mat[i] = expf(other[i]);
        sum += expf(other[i]);
    }
    return (1/sum)*new_mat;
}




