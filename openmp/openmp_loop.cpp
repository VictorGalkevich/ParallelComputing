#include <iostream>
#include <omp.h>
static long num_steps = 100000;
double step;
#define NUM_THREADS 2

void main() {
    double sum = 0.0;
    step = 1.0 / static_cast<double>(num_steps);
    omp_set_num_threads(NUM_THREADS);
#pragma omp parallel for private(x) reduction (+:sum)
    for (int i = 0; i < num_steps; i++) {
        const double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    double pi = sum * step;
    std::cout << pi;
}
