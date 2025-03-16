#include "string"
#include "Matrix.h"
#include <iostream>
#include <cmath>

#define INVALID_PARAMS "rows and columns neads to be positive"
#define INVALID_SIZES "the matrices are not the same size"
#define INVALID_INDEX "the given index is not valid"
#define INVALID_SIZE_FILE "the size of the file isn't match the size of the "\
"matrix"
#define BINARY_READ_FAILURE "couldn't read from the file"
#define MIN_VAL 0.1
/**
 * constructor with values
 * @param row
 * @param col
 */
Matrix::Matrix(int row, int col) : _rows(row), _cols(col) {
    if (_cols <= 0 || _rows <= 0) {
        throw std::runtime_error(INVALID_PARAMS);
    }
    this->ary_mat = new float[_rows * _cols]; // auto exception raised
    for (int i = 0; i < _rows * _cols; i++) {
        ary_mat[i] = 0;
    }
}

// default ctor
Matrix::Matrix() : Matrix(1,1){}

/**
 * copy constructor
 * the given parameter - other is const because we don't change it
 * @param other
 */
Matrix::Matrix(const Matrix &other) {
    ary_mat = new float[other._rows * other._cols];
    _rows = other._rows;
    _cols = other._cols;
    // deep copy
    for (int i = 0; i < other._rows; ++i) {
        for (int j = 0; j < other._cols; ++j) {
            ary_mat[(i * other._cols) + j] =
                    other.ary_mat[(i * other._cols) +j];
        }
    }
}

// destructor
Matrix::~Matrix() { delete[] ary_mat; }

int Matrix::get_rows() const { return this->_rows; }

int Matrix::get_cols() const { return this->_cols; }

Matrix &Matrix::transpose() {
    float *tmp = new float[_rows * _cols];
    for (int i=0; i< _rows * _cols; i++){
        tmp[i] = ary_mat[i];
    }
    int tmp1;
    tmp1 = _rows;
    _rows = _cols;
    _cols = tmp1;

    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) {
            ary_mat[(i * _cols) + j] = tmp[(j * _rows) + i];
        }
    }
    delete[] tmp;
    return *this;
}

// the matrix already presented in one line in the way it implemented
Matrix &Matrix::vectorize() {
    int tmp;
    tmp = _cols;
    _cols = 1;
    _rows = _rows * tmp;
    return *this;
}

/**
 * its const function because we don't change anything - just printing
 */
void Matrix::plain_print() const {
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++)
        {
            std::cout << ary_mat[(i * _cols) + j] << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * - the given parameter is const because we dont change it
 * - there is no const at the beginning because we want to be able to make
 * operations that can change the returned value
 * - there is not "& at the beginning because we return new object and not this
 * @param other
 * @return
 */
Matrix Matrix::dot(const Matrix &other) const {
    if (this->_rows != other._rows || this->_cols != other._cols) {
        throw std::domain_error(INVALID_SIZES);
    }
    // creating new Matrix object with the same size as this and other
    Matrix new_matrix(_rows, _cols);
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j)
        {new_matrix.ary_mat[(i * _cols) + j] =
                    this->ary_mat[(i * _cols) + j] *
                    other.ary_mat[(i *_cols) +j];
        }
    }
    return new_matrix;
}

/**
 * const at the end because we dont change anything
 * @return
 */
float Matrix::sum() const {
    float sum_values = 0;
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) {
            sum_values += this->ary_mat[(i * _cols) + j];
        }
    }
    return sum_values;
}

float Matrix::norm() const {
    float tmp_sum = 0;
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            tmp_sum += powf(ary_mat[(i * _cols) + j], 2);
        }
    }
    return sqrtf(tmp_sum);
}

int Matrix::argmax() const {
    float max_val = 0;
    int index = 0;
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            if (max_val < this->ary_mat[(i * _cols) + j]) {
                max_val = this->ary_mat[(i * _cols) + j];
                index = (i * _cols) + j;
            }
        }
    }
    return index;
}


Matrix &Matrix::operator+=(const Matrix &other) {
    if (this->_rows != other._rows || this->_cols != other._cols) {
        throw std::domain_error(INVALID_SIZES);
    }
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            this->ary_mat[(i * _cols) + j] = (this->ary_mat[(i * _cols) +j]+
                    other.ary_mat[(i * _cols) + j]);
        }
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix &other) const {
    if (this->_rows != other._rows || this->_cols != other._cols){
        throw std::domain_error(INVALID_SIZES);
    }
    Matrix new_mat(this->_rows, this->_cols);
    for (int i=0; i< this->_rows; i++){
        for (int j=0; j<this->_cols; j++){
            new_mat.ary_mat[(i * _cols) + j] = this->ary_mat[(i * _cols) +j]
                    + other.ary_mat[(i * _cols) + j];
        }
    }
    return new_mat;
}


Matrix &Matrix::operator=(const Matrix &other) {
    if (this == &other) {
        return *this;
    }
    float *old_mat = ary_mat;
    ary_mat = new float[other._rows * other._cols];
    _rows = other._rows;
    _cols = other._cols;
    // deep copy
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            ary_mat[(i * other._cols) + j]
            = other.ary_mat[(i * other._cols) +j];
        }
    }
    delete[] old_mat;
    return *this;
}

Matrix Matrix::operator*(const Matrix &other) const {
    if (this->_cols != other._rows){
        throw std::domain_error(INVALID_SIZES);
    }
    Matrix new_mat(this->_rows, other._cols);
    for (int i=0; i< this->_rows; i++){
        for (int j=0; j< other._cols; j++){
            float sum = 0;
            for (int k=0; k < other._rows; k++){
                sum += (this->ary_mat[(i*this->_cols)+k]
                        *other.ary_mat[(k*other._cols)+j]);
            }
            new_mat.ary_mat[(i*other._cols)+j] = sum;
        }
    }
    return new_mat;
}

Matrix Matrix::operator*(float c) const {
    Matrix new_matrix(this->_rows, this->_cols);
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            new_matrix.ary_mat[(i * _cols) + j] = (this->ary_mat[(i * _cols)
                                                              +j]*c);
        }
    }
    return new_matrix;
}

Matrix operator*(float c, const Matrix &matrix) {
    return matrix * c;
}



float Matrix::operator()(int i, int j) const {
    if (0 > i || i >= this->_rows || j < 0 || j >=this->_cols){
        throw std::out_of_range(INVALID_INDEX);
    }
    return this->ary_mat[(i* this->_cols)+j];
}

float& Matrix::operator()(int i, int j) {
    if (0 > i || i >= this->_rows || j < 0 || j >=this->_cols) {
        throw std::out_of_range(INVALID_INDEX);
    }
    return this->ary_mat[(i* this->_cols)+j];
}

float Matrix::operator[](int i) const {
    if (0 > i || i >= this->_rows * this->_cols){
        throw std::out_of_range(INVALID_INDEX);
    }
    return this->ary_mat[i];
}

float& Matrix::operator[](int i) {
    if (0 > i || i >= this->_rows * this->_cols){
        throw std::out_of_range(INVALID_INDEX);
    }
    return this->ary_mat[i];
}

std::ostream &operator<<(std::ostream &out, Matrix &matrix) {
    for (int i=0; i < matrix._rows; i++){
        for (int j=0; j<matrix._cols; j++){
            if (matrix.ary_mat[(i*matrix._cols)+j] > MIN_VAL){
                out << "**";
            }
            else{
                out << "  ";
            }
        }
        out << std::endl;
    }
    return out;
}


std::istream &operator>>(std::istream &is, Matrix &matrix) {
    if (!is.good()){
        throw std::runtime_error(BINARY_READ_FAILURE);
    }
    for (int i=0; i < matrix.get_rows(); i++){
        for (int j=0; j < matrix.get_cols(); j++){
            if (!is.read((char *)&matrix(i,j), sizeof (float ))){
                throw std::runtime_error(BINARY_READ_FAILURE);
            }
        }
    }
    return is;
}

















