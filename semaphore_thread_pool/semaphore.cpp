#include <functional>
#include <iostream>
#include <mutex>
#include <valarray>
#include <vector>
#include <ctime>
#include <semaphore>
#include <thread>

struct SyncHolder {
    std::mutex m;
    size_t res = 0;
    bool term = false;
};

bool isPrime(const size_t x) {
    const auto top = static_cast<size_t>(sqrt(x));
    for (size_t i = 2; i <= top; i++) {
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}

void countPrimes(
    SyncHolder &holder,
    std::counting_semaphore<> &sem,
    const size_t lower,
    const size_t upper
) {
    size_t cnt = 0;
    for (size_t i = lower; i <= upper && !holder.term; i++) {
        if (isPrime(i)) {
            ++cnt;
        }
    }
    sem.release();
    while (!holder.m.try_lock()) {
    }
    holder.res += cnt;
    holder.m.unlock();
}

int main() {
    const clock_t start = clock();

    size_t maximumNumOfParallelThreads;
    std::cout << "Input the maximum number of parallel threads: ";
    std::cin >> maximumNumOfParallelThreads;

    std::counting_semaphore sem(maximumNumOfParallelThreads);

    SyncHolder sync_holder;

    size_t sizeOfTheTask;
    std::cout << "Input size of the task: ";
    std::cin >> sizeOfTheTask;

    size_t sizeOfTheSubtask;
    std::cout << "Input size of the subtask: ";
    std::cin >> sizeOfTheSubtask;

    if (sizeOfTheSubtask >= sizeOfTheTask) {
        std::cout << "Subtask isn't small enough";
        return 0;
    }


    const size_t numberOfTasks = sizeOfTheTask / sizeOfTheSubtask;

    std::vector<std::thread> threads;

    for (size_t i = 0; i < numberOfTasks; ++i) {
        while (!sem.try_acquire()) {}
        threads.emplace_back(
            countPrimes,
            std::ref(sync_holder),
            std::ref(sem),
            i * sizeOfTheSubtask,
            (i + 1) * sizeOfTheSubtask);
    }
    std::ranges::for_each(threads, std::mem_fn(&std::thread::join));
    std::cout << "The result is: " << sync_holder.res << std::endl;
    std::cout << static_cast<double>(clock() - start) / 1000000 << " seconds" << std::endl;
}
