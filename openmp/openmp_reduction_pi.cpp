#include <iostream>
#include <omp.h>
static long num_steps = 100000;
double step;

int main() {
    double sum = 0.0;
    step = 1.0 / static_cast<double>(num_steps);
#pragma omp parallel for num_threads(4) reduction(+:sum)
    for (int i = 0; i < num_steps; i++) {
        const double x = (i + 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }

    const double pi = step * sum;
    std::cout <<pi;
}
