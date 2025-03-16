#include "Matrix.h"
#include "cmath"
#ifndef ACTIVATION_H
#define ACTIVATION_H

// Insert activation namespace here...
namespace activation{
    Matrix relu(const Matrix& other);
    Matrix softmax(const Matrix& other);
}

#endif //ACTIVATION_H