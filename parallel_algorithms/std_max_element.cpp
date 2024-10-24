#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>

int main() {
    size_t size;
    std::cout << "Input the vector size: ";
    std::cin >> size;
    std::vector<int> numbers(size);
    for (int i = 0; i < size; ++i) {
        numbers[i] = i + 1;
    }
    clock_t start = clock();
    const auto max_elem = std::max_element(std::execution::par, numbers.begin(), numbers.end());
    std::cout << "Time taken: " << static_cast<double>(clock() - start) / 1000000 << " seconds\n";
    std::cout << "Max Element: " << *max_elem << std::endl;
    return 0;
}
