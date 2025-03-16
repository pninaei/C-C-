//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"

#define MLP_SIZE 4

/**
 * @struct digit
 * @brief Identified (by Mlp network) digit with
 *        the associated probability.
 * @var value - Identified digit value
 * @var probability - identification probability
 */
typedef struct digit {
    unsigned int value;
    float probability;
} digit;

const Matrix::dims img_dims = {28, 28};
const Matrix::dims weights_dims[] = {{128, 784},
                                     {64,  128},
                                     {20,  64},
                                     {10,  20}};
const Matrix::dims bias_dims[] = {{128, 1},
                                  {64,  1},
                                  {20,  1},
                                  {10,  1}};

// Insert MlpNetwork class here...

class MlpNetwork{
public:
    // Constructor //
    MlpNetwork(const Matrix *weights, const Matrix *biases);
    // Method //
    digit operator()(const Matrix& img) const;
private:
   Dense layer1;
   Dense layer2;
   Dense layer3;
   Dense layer4;
};

#endif // MLPNETWORK_H
