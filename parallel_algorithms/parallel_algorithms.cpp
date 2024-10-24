#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <execution>
#include <chrono>

using namespace std;

int increment(int &a) {
    a *= 10;
    return a;
}

int main()
{
    int n = 1'00'000'000;
    vector<int> vector(n);
    mt19937 gen;
    uniform_int_distribution<int> d(0, 1'000'000'000);
    auto rand_num ([=] () mutable { return d(gen); });

    clock_t start = clock();

    generate(execution::par, begin(vector), end(vector), rand_num);
    //    std::sort(execution::seq, begin(vector), end(vector));
    //    std::max_element(execution::seq, begin(vector), end(vector));
    //    std::for_each(execution::seq, begin(vector), end(vector), [](int &n) { increment(n); });
    std::min_element(execution::seq, begin(vector), end(vector));

    std::cout << "Time taken: " << static_cast<double>(clock() - start) / 1000000 << " seconds\n";
    return 0;
}
