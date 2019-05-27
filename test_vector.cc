#include "include/io.hh"
#include "include/sort.hh"
#include "include/vector.hh"

using wxg::print;
using wxg::push_random_n;

void test_vector() {
    std::vector<int> vec;
    wxg::push_random_n(vec, 100, -1000, 1000);
    wxg::print(vec);
    auto vec2 = vec;

    wxg::quick_sort(vec, 0, vec.size() - 1);
    wxg::print(vec);

    std::sort(vec2.begin(), vec2.end());
    print(vec2);

    if (vec == vec2) cout << "OK\n";
}

void test_permutation() {
    int n = 4, k = 9;
    vector<int> vec(n, 0);
    for (int i = 1; i <= n; i++) vec[i - 1] = i;
    wxg::print(vec);
    wxg::get_permutation(vec, k);
    wxg::print(vec);
    string s = "";
    for (int i = 0; i < n; i++) s += char('0' + vec[i]);
    cout << "s=" << s << std::endl;
    vector<int> input = {1, 2, 3, 4};
    wxg::get_permutation(input, 9);
    wxg::print(input, " ");
}

void test_array() {
    vector<int> vec;
    push_random_n(vec, 20, 0, 10);
    std::sort(vec.begin(), vec.end());

    print(vec);

    cout << wxg::unique(vec) << std::endl;
    print(vec);

    wxg::random_shuffle(vec, 0, vec.size());
    print(vec);
}

void test_sort() {
    vector<int> vec;
    vec = {1, 5, 3, 7, 2};
    push_random_n(vec, 50, 0, 100);
    auto vec2 = vec;
    // print(vec);

    // wxg::partial_sort(vec, 0, 10, vec.size());
    // std::partial_sort(vec2.begin(), vec2.begin()+10, vec2.end());
    // print(vec);
    // print(vec2);

    std::sort(vec.begin(), vec.end());
    // wxg::bubble_sort(vec2, 0, vec2.size());
    // wxg::insert_sort(vec2, 0, vec2.size());
    // wxg::select_sort(vec2, 0, vec2.size());
    // wxg::quick_sort(vec2, 0, vec2.size());
    // wxg::sort(vec2, 0, vec2.size());
    wxg::shell_sort(vec2, 0, vec2.size());

    // print(vec2);

    if (vec != vec2)
        cout << "error" << std::endl;
    else
        cout << "OK\n";
}

int main(int argc, char const *argv[]) {
    test_permutation();
    test_vector();
    test_array();
    test_sort();
    return 0;
}
