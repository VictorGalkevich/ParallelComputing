#include <thread>
#include <iostream>
#include <time.h>

constexpr size_t N = 1000000;
std::mutex m;
size_t res = 0;

bool isPrime(const size_t x) {
    const long double top = std::sqrt(x);
    for (size_t i = 2; i <= top; i++) {
        if ( x % i == 0 ) {
            return false;
        }
    }
    return true;
}

void countPrimes(const int32_t min, const int32_t max) {
    size_t cnt = 0;
    for (size_t i = min; i <= max; i++) {
        if (isPrime(i)) {
            ++cnt;
        }
    }
    while(!m.try_lock()) {}
    res += cnt;
    m.unlock();
}

int main() {
    const clock_t t1 = clock();

    int32_t threads;

    std::cin >> threads;

    const int32_t step = N / threads;

    for (size_t i = 0; i < threads; ++i) {
        size_t min = i * step;
        size_t max = (i + 1) * step;
        std::thread t(countPrimes, std::ref(min), std::ref(max));
        t.join();
    }

    std::cout << res << std::endl;
    std::cout << static_cast<double>(clock() - t1) / 1000000 << " seconds";

    return 0;
}
