#include <omp.h>
#include <iostream>
static long num_steps = 100000;
double step;
#define NUM_THREADS 2

void main() {
    int i, nthreads = 0;
    double pi, sum[NUM_THREADS];
    step = 1.0 / static_cast<double>(num_steps);
    omp_set_num_threads(NUM_THREADS);
#pragma omp parallel
    {
        int i1;
        const int id = omp_get_thread_num();
        const int nthrds = omp_get_num_threads();
        if (id == 0) nthreads = nthrds;
        for (i1 = id, sum[id] = 0.0; i1 < num_steps; i1 = i1 + nthrds) {
            double x = (i1 + 0.5) * step;
            sum[id] += 4.0 / (1.0 + x * x);
        }
    }
    for (i = 0, pi = 0.0; i < nthreads; i++) {
        pi += sum[i] * step;
    }

    std::cout << pi;
}
