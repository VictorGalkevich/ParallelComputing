#include <iostream>
#include <thread>
#include <valarray>

size_t res = 0;

bool isPrime(const size_t x) {
    const long double top = std::sqrt(x);
    for (size_t i = 2; i <= top; i++) {
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    auto my_thread = std::jthread(
        [](const std::stop_token &stop_token) {
            size_t cnt = 0;
            for (size_t i = 2; i <= 100'000'000 && !stop_token.stop_requested(); i++) {
                if (isPrime(i)) {
                    ++cnt;
                }
            }
            res = cnt;
        }
    );

    std::string stopword;
    while (!my_thread.get_stop_source().stop_requested()) {
        if (std::cin >> stopword) {
            my_thread.request_stop();
        }
    }

    my_thread.join();
    std::cout << res;
    return 0;
}
