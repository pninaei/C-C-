#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"
typedef Matrix (*active_func)(const Matrix& other);

class Dense{
public:
    Dense(const Matrix& weights, const Matrix& bias, active_func func);
    // Methods //

    const Matrix& get_weights() const;
    const Matrix& get_bias() const;
    const active_func& get_activation() const;
    // Operator //
    Matrix operator()(const Matrix& vector) const;

private:
    Matrix _weights;
    Matrix _bias;
    active_func _func;
};


#endif //DENSE_H
