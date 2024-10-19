#include <condition_variable>
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
using namespace std;

class ThreadPool {
public:
    explicit ThreadPool(const size_t num_threads)
    {
        for (size_t i = 0; i < num_threads; ++i) {
            threads.emplace_back([this] {
                while (!term) {
                    while (!qm.try_lock()) {}
                    if (!tasks.empty()) {
                        std::function<void()> task;
                        task = move(tasks.front());
                        tasks.pop();
                        task();
                    }
                    qm.unlock();
                }
            });
        }
    }

    ~ThreadPool()
    {
        for (auto & thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    void enqueue(function<void()> task)
    {
        while (!qm.try_lock()) {}
        tasks.emplace(move(task));
        qm.unlock();
    }

    void stop() {
        while (!qm.try_lock()) {}
        term = true;
        qm.unlock();
        std::ranges::for_each(threads, std::mem_fn(&std::thread::join));
    }

private:
    vector<thread> threads;

    queue<function<void()> > tasks;

    mutex qm;

    bool term = false;
};

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
    const size_t lower,
    const size_t upper
) {
    size_t cnt = 0;
    for (size_t i = lower; i <= upper && !holder.term; i++) {
        if (isPrime(i)) {
            ++cnt;
        }
    }
    while (!holder.m.try_lock()) {
    }
    holder.res += cnt;
    holder.m.unlock();
}

int main()
{
    const clock_t start = clock();

    size_t poolSize;
    std::cout << "Input the pool size: ";
    std::cin >> poolSize;

    ThreadPool pool(poolSize);

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

    for (size_t i = 0; i < numberOfTasks; ++i) {
        const size_t lower = (i) * sizeOfTheSubtask;
        const size_t upper = (i + 1) * sizeOfTheSubtask;
        pool.enqueue([&sync_holder, lower, upper] {
            countPrimes(sync_holder, lower, upper);
        });
    }

    pool.stop();
    std::cout << static_cast<double>(clock() - start) / 1000000 << " seconds" << std::endl;
    std::cout << sync_holder.res << std::endl;

    return 0;
}
