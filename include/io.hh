#include <chrono>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using Clock = std::chrono::high_resolution_clock;
using Task = std::function<void()>;

using namespace std::chrono;

using std::cout;
using std::string;
using std::vector;

namespace wxg {

template <typename Iterator>
void print(const Iterator &begin, const Iterator &end, const char *delimiter) {
    using T = typename Iterator::value_type;
    std::ostream_iterator<T> outIter(std::cout, delimiter);
    std::copy(begin, end, outIter);
    std::cout << std::endl;
}

template <typename T>
void print(const std::vector<T> &vec, const char *delimiter) {
    print(vec.begin(), vec.end(), delimiter);
}

template <typename T>
void print(const std::vector<T> &vec) {
    print(vec, " ");
}

template <typename T>
void push_random_n(T &t, size_t n, int min, int max) {
    int range = max - min;
    for (size_t i = 0; i < n; i++) t.push_back(min + rand() % range);
}

template <typename T>
void insert_random_n(T &t, size_t n, int min, int max) {
    int range = max - min;
    for (size_t i = 0; i < n; i++) t.insert(min + rand() % range);
}

void run_task(Task &&tsk) {
    auto t1 = Clock::now();
    tsk();
    auto t2 = Clock::now();

    auto time_span = duration_cast<duration<double>>(t2 - t1);

    std::cout << "meansure time:" << time_span.count() << std::endl;
}

}  // namespace wxg