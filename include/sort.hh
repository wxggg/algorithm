#include <vector>

using std::vector;

namespace wxg {

template <typename T>
void bubble_sort(vector<T> &vec, int first, int last) {
    for (int i = first; i != last; i++) {
        for (int j = last - 1; j != first; j--)
            if (vec[j] < vec[j - 1]) std::swap(vec[j], vec[j - 1]);
    }
}

template <typename T>
void select_sort(vector<T> &vec, int first, int last) {
    int minIdx;
    for (int i = first; i != last; i++) {
        minIdx = i;
        for (int j = i; j != last; j++)
            if (vec[j] < vec[minIdx]) minIdx = j;
        std::swap(vec[i], vec[minIdx]);
    }
}

template <typename T>
void insert_sort(vector<T> &vec, int first, int last) {
    for (int i = first + 1; i != last; i++) {
        if (vec[i] < vec[i - 1]) {
            T value = vec[i];
            int j;
            for (j = i; j > first && vec[j - 1] > value; j--)
                vec[j] = vec[j - 1];
            vec[j] = value;
        }
    }
}

template <typename T>
void shell_insert(vector<T> &vec, int first, int last, int dk) {
    for (int i = first + dk; i < last; i++) {
        if (vec[i] < vec[i - dk]) {
            T value = vec[i];
            int j;
            for (j = i; j - dk >= first && vec[j - dk] > value; j -= dk)
                vec[j] = vec[j - dk];
            vec[j] = value;
        }
    }
}

template <typename T>
void shell_sort(vector<T> &vec, int first, int last) {
    int dk = (last - first) / 2;
    while (dk >= 1) {
        shell_insert(vec, first, last, dk);
        dk >>= 1;
    }
}

template <typename T, typename Predicate>
int partition(vector<T> &vec, int first, int last, Predicate pred) {
    --last;
    while (first < last) {
        while (first < last && pred(vec[first])) ++first;
        while (first < last && !pred(vec[last])) --last;
        std::swap(vec[first], vec[last]);
    }
    return first;
}

template <typename T>
int partition(vector<T> &vec, int first, int last, const T &pivot) {
    while (true) {
        while (vec[first] < pivot) ++first;
        --last;
        while (pivot < vec[last]) --last;
        if (!(first < last)) return first;
        std::swap(vec[first], vec[last]);
        ++first;
    }
    return first;
}

/** use vec[first] as pivot
 * after partition pivot will be in the right place */
template <typename T>
int partition(vector<T> &vec, int first, int last) {
    T pivot = vec[first];
    --last;
    while (first < last) {
        while (first < last && vec[last] >= pivot) --last;
        std::swap(vec[first], vec[last]);
        while (first < last && vec[first] <= pivot) ++first;
        std::swap(vec[first], vec[last]);
    }
    return first;
}

template <class T>
inline const T &median(const T &a, const T &b, const T &c) {
    if (a < b) {
        if (b < c) return b;
        if (a < c) return c;
        return a;
    } else {  // b < a
        if (a < c) return a;
        if (b < c) return c;
        return b;
    }
}

template <typename T>
void quick_sort(vector<T> &vec, int first, int last) {
    if (first < last - 1) {
        int pivotIndex = partition(vec, first, last);
        quick_sort(vec, first, pivotIndex);
        quick_sort(vec, pivotIndex + 1, last);
    }
}

template <typename T>
void sort(vector<T> &vec, int first, int last) {
    if (last - first > 16) {
        auto pivot =
            median(vec[first], vec[first + (last - first) / 2], vec[last - 1]);
        int pivotIndex = partition(vec, first, last, pivot);
        sort(vec, first, pivotIndex);
        sort(vec, pivotIndex, last);
    } else
        insert_sort(vec, first, last);
}

/** the nth value will be on the nth place
 * and left <= nth <= right */
template <typename T>
void nth_element(vector<T> &vec, int first, int nth, int last) {
    while (last - first > 3) {
        auto pivot =
            median(vec[first], vec[first + (last - first) / 2], vec[last - 1]);
        auto cut = partition(vec, first, last, pivot);
        if (cut <= nth)
            first = cut;
        else
            last = cut;
    }
    insert_sort(vec, first, last);
}

}  // namespace wxg