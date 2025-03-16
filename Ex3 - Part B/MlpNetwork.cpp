#include "Matrix.h"
#include "MlpNetwork.h"
#include "Dense.h"

#define INVALID_SIZES "the matrices are not the same size"
#define MAX_INDEX 10

MlpNetwork::MlpNetwork(const Matrix *weights, const Matrix *biases) :
layer1(weights[0], biases[0], activation::relu),
layer2(weights[1], biases[1], activation::relu),
layer3(weights[2], biases[2], activation::relu),
layer4(weights[3], biases[3], activation::softmax)
{}

digit MlpNetwork::operator()(const Matrix &img) const {

    Matrix mat(img);
    mat.vectorize();
    Matrix out1 = layer1(mat);
    Matrix out2 = layer2(out1);
    Matrix out3 = layer3(out2);
    Matrix output = layer4(out3);

    digit max_val = {0,0};
    for (int i=0; i < MAX_INDEX; i++){
        if (output[i] > max_val.probability){
            max_val.probability = output[i];
            max_val.value = i;
        }
    }
    return max_val;
}

