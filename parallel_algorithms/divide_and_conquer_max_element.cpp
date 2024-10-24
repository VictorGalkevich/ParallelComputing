#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <future>

template<typename Iterator>
Iterator parallel_max_element(Iterator first, Iterator last) {
    auto length = std::distance(first, last);
    if (length < 1000) {
        return std::max_element(first, last);
    }

    Iterator mid = first;
    std::advance(mid, length / 2);

    auto handle = std::async(std::launch::async, parallel_max_element<Iterator>, mid, last);
    Iterator max_elem_first = parallel_max_element(first, mid);
    Iterator max_elem_second = handle.get();

    return *max_elem_first > *max_elem_second ? max_elem_first : max_elem_second;
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
    const auto result = parallel_max_element(numbers.begin(), numbers.end());
    std::cout << "Max Element: " << *result << std::endl;
    std::cout << "Time taken: " << static_cast<double>(clock() - start) / 1000000 << " seconds\n";
    return 0;
}
