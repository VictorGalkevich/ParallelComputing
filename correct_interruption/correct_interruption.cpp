#include <iostream>
#include <thread>
#include <string>

size_t res = 0;
bool term = false;

bool isPrime(const size_t x) {
    const long double top = std::sqrt(x);
    for (size_t i = 2; i <= top; i++) {
        if ( x % i == 0 ) {
            return false;
        }
    }
    return true;
}

void countPrimes() {
    size_t cnt = 0;
    for (size_t i = 2; i <= 10'000'000 && !term; i++) {
        if (isPrime(i)) {
            ++cnt;
        }
    }
    term = true;
    res = cnt;
}

int main() {
    std::thread t(countPrimes);

    std::string word;
    while (!term) {
        if (std::cin >> word) {
            term = true;
        }
    }

    t.join();

    std::cout << res;
}
