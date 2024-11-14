#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <algorithm>
#include <functional>

void reverse_words(
    std::vector<std::string> &words,
    const size_t start,
    const size_t end) {
    for (size_t i = start; i < end; ++i) {
        std::ranges::reverse(words[i]);
    }
}

int main() {
    std::string input_filename = "input.txt";
    std::string output_filename = "output.txt";

    std::ifstream input_file(input_filename);

    if (!input_file.is_open()) {
        std::cerr << "Unable to open the file: " << input_filename << std::endl;
        return 1;
    }

    std::vector<std::string> words;
    std::string word;
    while (input_file >> word) {
        words.push_back(word);
    }
    input_file.close();

    const unsigned int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    size_t total_words = words.size();
    size_t chunk_size = total_words / num_threads;

    for (unsigned int i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_threads - 1) ? total_words : start + chunk_size;
        threads.emplace_back(reverse_words, std::ref(words), start, end);
    }

    std::ranges::for_each(threads, std::mem_fn(&std::thread::join));

    std::ofstream output_file(output_filename);
    if (!output_file.is_open()) {
        std::cerr << "Unable to open the file: " << output_filename << std::endl;
        return 1;
    }

    for (const auto &reversed_word: words) {
        output_file << reversed_word << " ";
    }
    output_file.close();

    std::cout << "Data was written to the file: " << output_filename << std::endl;

    return 0;
}
