#include "include/hashtable.hh"
#include "include/heap.hh"
#include "include/io.hh"
#include "include/list.hh"

#include <algorithm>
#include <list>

using std::cout;
using std::endl;

using wxg::print;
using wxg::push_random_n;
using wxg::run_task;

void test_list() {
    std::list<int> li2;
    li2.push_back(1);
    push_random_n(li2, 500000, 0, 100000);

    run_task([&li2]() { li2.sort(); });

    wxg::list<int> li1;
    li1.push_back(1);
    push_random_n(li1, 500000, 0, 100000);

    // li1.tranverse([](int& x) { cout<<x<<","; });
    // cout<<endl;

    run_task([&li1]() {
        li1.sort();
        // li1.insert_sort();
    });
}

void test_heap() {
    // vector<int> vec1 = {3, 9, 10, 5, 2, 6, 8, 11};
    vector<int> vec1;
    push_random_n(vec1, 20, 0, 10);
    auto vec2 = vec1;
    auto vec3 = vec1;

    // wxg::make_heap(vec1, 0, vec1.size());

    // make_heap(vec2.begin(), vec2.end());

    // print(vec1);
    // wxg::pop_heap(vec1, 0, vec1.size());
    // vec1.pop_back();
    // print(vec1);

    // wxg::push_heap(vec1, 12);
    wxg::make_heap(vec1);
    wxg::sort_heap(vec1, 0, vec1.size());
    print(vec1);

    // print(vec2);
    // pop_heap(vec2.begin(), vec2.end());
    // vec2.pop_back();
    // print(vec2);
    // vec2.push_back(12);
    // push_heap(vec2.begin(), vec2.end());
    std::make_heap(vec2.begin(), vec2.end());
    sort_heap(vec2.begin(), vec2.end());
    print(vec2);

    std::sort(vec3.begin(), vec3.end());
    print(vec3);
}

void test_hashtable() {
    wxg::hashtable<int, int> ht;
    insert_random_n(ht, 100, 0, 1000);

    // for(int i=0; i<54; i++)
    // {
    //     ht.insert(i);
    // }

    ht.traverse();
}

int main(int argc, char const *argv[]) {
    // test_list();

    // test_heap();

    test_hashtable();

    return 0;
}
