#include <algorithm>
#include <functional>
#include <set>
#include <vector>

using std::vector;

namespace wxg {

template <typename T>
int lower_bound(const vector<T> &vec, const T &value) {
    if (vec.empty()) return 0;
    int left = 0, right = vec.size() - 1, mid;
    if (vec[right] < value) return vec.size();
    while (vec[left] < value && left < right) {
        mid = (left + right) / 2;
        if (vec[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

template <typename T>
bool binary_search(const vector<T> &vec, const T &value) {
    int i = lower_bound(vec, value);
    return i != vec.size() && vec[i] == value;
}

template <typename T>
bool next_permutation(vector<T> &vec) {
    int n = vec.size();
    if (n <= 1) return false;
    for (int i = n - 1; i > 0; i--) {
        if (vec[i] > vec[i - 1]) {
            int j = n - 1;
            while (vec[j] <= vec[i - 1]) --j;
            std::swap(vec[j], vec[i - 1]);
            std::reverse(vec.begin() + i, vec.end());
            return true;
        }
    }
    std::reverse(vec.begin(), vec.end());
    return false;
}

/** get Kth permutation */
template <typename T>
void get_permutation(vector<T> &vec, int k) {
    std::sort(vec.begin(), vec.end());
    int n = vec.size();
    int count = 1;
    for (int i = n; i > 1; i--) count *= i;

    for (auto first = vec.begin(); n > 0; --n, ++first) {
        count /= n;
        --k;
        auto d = first + k / count;
        std::rotate(first, d, d + 1);
        k %= count;
        ++k;
    }
}

template <typename T>
int unique(vector<T> &vec) {
    if (vec.empty()) return 0;
    int i = 0, j = 0;
    while (++j < vec.size())
        if (vec[i] != vec[j]) vec[++i] = vec[j];
    return i + 1;
}

template <typename T>
void remove_duplicates(vector<T> &vec) {
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}

template <typename T>
vector<vector<int>> permute(vector<T> &vec) {
    std::sort(vec.begin(), vec.end());
    vector<vector<int>> res;
    res.push_back(vec);
    while (next_permutation(vec)) res.push_back(vec);
    return res;
}

/** [first, last) */
template <typename T>
void random_shuffle(vector<T> &vec, int first, int last) {
    if (vec.empty() || first == last) return;
    for (int i = first; i < last; i++)
        std::swap(vec[i], vec[first + std::rand() % ((i - first) + 1)]);
}

}  // namespace wxg
