#include <iostream>
#include <omp.h>
static long num_steps = 100000000;
#define MIN_BLK 10000000

double pi_comp(const int Nstart, const int Nfinish, const double step) {
    double sum = 0.0;
    if (Nfinish - Nstart < MIN_BLK) {
        for (int i = Nstart; i < Nfinish; i++) {
            const double x = (i + 0.5) * step;
            sum = sum + 4.0 / (1.0 + x * x);
        }
    } else {
        const int iblk = Nfinish - Nstart;

#pragma omp task shared(sum1)
        const double sum1 = pi_comp(Nstart, Nfinish - iblk / 2, step);

#pragma omp task shared(sum2)
        const double sum2 = pi_comp(Nfinish - iblk / 2, Nfinish, step);

#pragma omp taskwait
        sum = sum1 + sum2;
    }
    return sum;
}

int main ()
{
    int i;
    double sum;
    const double step = 1.0 / static_cast<double>(num_steps);
#pragma omp parallel
    {
#pragma omp single
        sum = pi_comp(0,num_steps,step);
    }
    double pi = step * sum;
    std::cout << pi;
}
