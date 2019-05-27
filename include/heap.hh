#include <iostream>
#include <vector>

namespace wxg {

inline int left(int index) { return 2 * index + 1; }
inline int right(int index) { return 2 * index + 2; }
inline int parent(int index) { return (index - 1) / 2; }

template <typename T>
void push_heap(std::vector<T> &vec, int first, int holeIndex, int topIndex,
               T value) {
    while (holeIndex > topIndex && vec[first + parent(holeIndex)] < value) {
        vec[first + holeIndex] = vec[first + parent(holeIndex)];
        holeIndex = parent(holeIndex);
    }
    vec[first + holeIndex] = value;
}

template <typename T>
void push_heap(std::vector<T> &vec, const T &value) {
    vec.push_back(value);
    push_heap(vec, 0, vec.size() - 1, 0, value);
}

template <typename T>
void adjust_heap(std::vector<T> &vec, int first, int holeIndex, int len,
                 T value) {
    int topIndex = holeIndex;
    int secondChild = right(holeIndex);  // right child
    while (secondChild < len) {
        if (vec[first + secondChild] <
            vec[first + secondChild - 1])  // right < left
            secondChild--;                 // second store the bigger
        vec[first + holeIndex] = vec[first + secondChild];
        holeIndex = secondChild;
        secondChild = right(holeIndex);
    }
    if (secondChild == len) {  // only left child
        vec[first + holeIndex] = vec[first + secondChild - 1];
        holeIndex = secondChild - 1;
    }
    push_heap(vec, first, holeIndex, topIndex, value);
}

// [first, last)
template <typename T>
void make_heap(std::vector<T> &vec, int first, int last) {
    if (last - first < 2) return;
    int len = last - first;
    int holeIndex = parent(len - 1);

    while (true) {
        adjust_heap(vec, first, holeIndex, len, vec[first + holeIndex]);
        if (holeIndex == 0) return;
        holeIndex--;
    }
}

template <typename T>
void make_heap(std::vector<T> &vec) {
    make_heap(vec, 0, vec.size());
}

// [first, last)
template <typename T>
void pop_heap(std::vector<T> &vec, int first, int last) {
    auto value = vec[last - 1];
    vec[last - 1] = vec[first];
    adjust_heap(vec, first, 0, last - first - 1, value);
}

template <typename T>
void pop_heap(std::vector<T> &vec, int first, int last, T &x) {
    std::swap(vec[first], x);
    adjust_heap(vec, first, 0, last - first, vec[first]);
}

template <typename T>
void pop_heap(std::vector<T> &vec) {
    pop_heap(vec, 0, vec.size());
    vec.pop_back();
}

// [first, last)
template <typename T>
void sort_heap(std::vector<T> &vec, int first, int last) {
    while (last - first > 1) pop_heap(vec, first, last--);
}

template <typename T>
void partial_sort(vector<T> &vec, int first, int middle, int last) {
    make_heap(vec, first, middle);
    for (int i = middle; i < last; i++)
        if (vec[i] < vec[first]) pop_heap(vec, first, middle, vec[i]);
    sort_heap(vec, first, middle);
}

}  // namespace wxg
