#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <functional>
#include <numeric>

template<typename Iterator, typename T>
void max_element_block(Iterator first, Iterator last, T& result) {
    result = *std::max_element(first, last);
}

template<typename Iterator, typename T>
T parallel_max_element(Iterator first, Iterator last, T init) {
    unsigned long const length = std::distance(first, last);

    if (!length)
        return init;

    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;

    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_threads ? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads;

    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1);

    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(max_element_block<Iterator, T>, block_start, block_end, std::ref(results[i]));
        block_start = block_end;
    }
    max_element_block(block_start, last, results[num_threads - 1]);

    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    return *std::max_element(results.begin(), results.end());
}

int main() {
    size_t size;
    std::cout << "Input the vector size: ";
    std::cin >> size;
    std::vector<int> numbers(size);
    for (int i = 0; i < size; ++i) {
        numbers[i] = i + 1;
    }
    clock_t start = clock();
    const int result = parallel_max_element(numbers.begin(), numbers.end(), std::numeric_limits<int>::min());
    std::cout << "Time taken: " << static_cast<double>(clock() - start) / 1000000 << " seconds\n";
    std::cout << "Max Element: " << result << std::endl;
    return 0;
}
