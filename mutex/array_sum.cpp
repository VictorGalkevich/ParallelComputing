//
// Created by Victor Galkevich on 9/26/24.
//

#include <iostream>
#include <thread>
#include <vector>

constexpr int32_t N = 1000000000;
std::mutex m;
int32_t res = 0;


int main() {
    int32_t threads;

    std::cin >> threads;
    const int32_t step = N / threads;

    std::vector<int32_t> vec(N);
    const clock_t t1 = clock();
    std::vector<std::thread> trs(threads);

    for (int32_t i = 0; i < N; ++i) {
        vec[i] = i + 1;
    }
    std::cout << static_cast<double>(clock() - t1) / 1000000 << " seconds";

    for (int32_t i = 0; i < threads; ++i) {
        int32_t min = i * step;
        int32_t max = (i + 1) * step;

        trs.emplace_back(
            [] (const std::vector<int32_t>* vector, const int32_t start, const int32_t end) {
                int32_t tmp = 0;

                for (int32_t j = start; j < end; ++j) {
                    tmp += (*vector)[j];
                }
                while (!m.try_lock()){}
                res += tmp;
                m.unlock();
            }, &vec, std::ref(min), std::ref(max));
    }

    std::ranges::for_each(trs, std::mem_fn(&std::thread::join));
    std::cout << res << std::endl;
    std::cout << static_cast<double>(clock() - t1) / 1000000 << " seconds";


    return 0;
}
