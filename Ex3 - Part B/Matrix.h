
#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>

class Matrix {

public:
    /**
     * @struct dims
     * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
     */

    Matrix(int row, int col); // constructor
    Matrix(); // default constructor
    Matrix(const Matrix &other); // copy constructor
    ~Matrix(); // destructor

    // *** Methods & Function *** //
    //            ***             //

    int get_rows() const; // return the amount of rows as int
    int get_cols() const; // return the amount of cols as int
    Matrix& transpose(); // return mat as transpose
    Matrix& vectorize();
    void plain_print() const; // print matrix
    Matrix dot(const Matrix& other) const; // element-wise multiplication mat
    float sum() const; // sum of matrix
    float norm() const; // norm of the matrix
    int argmax() const; // max value in the matrix

    // *** Operators *** //
    //       ***         //
    Matrix& operator+=(const Matrix& other); // mat addition accumulation
    Matrix operator+(const Matrix& other) const; // matrix addition
    Matrix& operator=(const Matrix& other); // assigment
    Matrix operator*(const Matrix& other) const; // mat multiplication
    Matrix operator*(float c) const; // scalar multiplication from right
    friend Matrix operator*(float c, const Matrix& matrix);
    float operator()(int i, int j) const;
    float& operator()(int i, int j);
    float operator[](int i) const;
    float& operator[](int i);
    friend std::ostream& operator<<(std::ostream& out, Matrix& matrix);
    friend std::istream& operator>>(std::istream& in, Matrix& matrix);

    struct dims {
        int rows, cols;
    };
private:
    int _rows;
    int _cols;
    float* ary_mat;
};

#endif //MATRIX_H
