#include <iostream>
#include <omp.h>

int main() {
  const long long num_steps = 1000000000;
  constexpr double step = 1.0 / static_cast<double>(num_steps);
  double sum = 0.0;


#pragma omp parallel for private(x) reduction(+:sum)
  for (long long i = 0; i < num_steps; ++i) {
    const double x = (i + 0.5) * step;
    sum += 4.0 / (1.0 + x * x);
  }

  const double pi = step * sum;
  std::cout << "PI = " << pi << std::endl;

  return 0;
}