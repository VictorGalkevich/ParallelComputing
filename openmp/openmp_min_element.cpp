#include <iostream>
#include <vector>
#include <omp.h>
#include <limits>
#include <random>

int main() {
    const size_t vector_size = 1000000;
    std::vector<int> data(vector_size);

#pragma omp parallel
    {
        std::mt19937 generator(omp_get_thread_num());
        std::uniform_int_distribution<int> distribution(1, 1000000);

#pragma omp for
        for (size_t i = 0; i < vector_size; ++i) {
            data[i] = distribution(generator);
        }
    }

    int min_value = std::numeric_limits<int>::max();

#pragma omp parallel for reduction(min:min_value)
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] < min_value) {
            min_value = data[i];
        }
    }

    std::cout << "Min value: " << min_value << std::endl;

    return 0;
}