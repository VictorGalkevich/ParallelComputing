#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <atomic>

void calculate_pi(
    const int num_samples,
    std::atomic<int>& inside_circle,
    std::counting_semaphore<> &sem) {
    std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    int count = 0;
    for (int i = 0; i < num_samples; ++i) {
        double x = distribution(generator);
        double y = distribution(generator);
        if (x * x + y * y <= 1.0) {
            ++count;
        }
    }
    inside_circle += count;
}

int main() {
    const int num_threads = std::thread::hardware_concurrency();
    constexpr int total_samples = 10000000;
    const int samples_per_thread = total_samples / num_threads;
    std::counting_semaphore sem(num_threads);

    std::vector<std::thread> threads;
    std::atomic<int> inside_circle(0);

    for (int i = 0; i < num_threads; ++i) {
        sem.acquire();
        threads.emplace_back(
            calculate_pi,
            samples_per_thread,
            std::ref(inside_circle),
            std::ref(sem)
            );
    }

    std::ranges::for_each(threads, std::mem_fn(&std::thread::join));

    double pi = 4.0 * inside_circle / total_samples;
    std::cout << "PI: " << pi << std::endl;

    return 0;
}