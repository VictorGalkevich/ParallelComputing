#include <iostream>
#include <vector>
#include <omp.h>
#include <random>

int main() {
    const size_t vector_size = 1000000;
    std::vector<double> vecA(vector_size);
    std::vector<double> vecB(vector_size);

    std::mt19937 generator(42);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

#pragma omp parallel for
    for (size_t i = 0; i < vector_size; ++i) {
        vecA[i] = distribution(generator);
        vecB[i] = distribution(generator);
    }

    double dot_product = 0.0;

#pragma omp parallel for reduction(+:dot_product)
    for (size_t i = 0; i < vector_size; ++i) {
        dot_product += vecA[i] * vecB[i];
    }

    std::cout << "Dot product: " << dot_product << std::endl;

    return 0;
}