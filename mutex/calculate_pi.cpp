//
// Created by Victor Galkevich on 9/26/24.
//

#include <thread>
#include <iostream>
#include <ctime>
#include <__format/format_functions.h>

constexpr size_t N = 1000000000;
std::mutex m;
long double res = 0;

void calcPi(long double min, const long double max, const long double delta) {
    long double tmp = 0;

    while(min < max) {
        tmp += 4 / (1 + min * min);
        min += delta;
    }

    while(!m.try_lock()) {}
    res += tmp;
    m.unlock();
}

int main() {
    const clock_t t1 = clock();

    size_t threads;

    std::cin >> threads;

    const long double size = 1.0 / threads;
    const long double delta = 1.0 / N;

    for (size_t i = 0; i < threads; ++i) {
        long double min =  i * size;
        long double max = (i + 1) * size;
        std::thread t(calcPi, std::ref(min), std::ref(max), std::ref(delta));
        t.join();
    }

    std::cout << std::format("{:.10f}", res / N) << std::endl;
    std::cout << static_cast<double>(clock() - t1) / 1000000 << " seconds";

    return 0;
}


