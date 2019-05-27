#include "include/btree.hh"
#include "include/io.hh"
#include "include/tree.hh"

#include <algorithm>
#include <list>

using std::cout;
using std::endl;

using wxg::print;
using wxg::push_random_n;
using wxg::run_task;

void test_avl_tree() {
    wxg::avl_tree<int, int> avl;
    // insert_random_n(avl, 10, 0, 10);

    for (int i = 0; i < 10; i++) {
        avl.insert(i);
        avl.traverse();
    }
    avl.traverse();

    for (int i = 0; i < 10; i++) {
        avl.remove(i);
        avl.traverse();
    }
}

void test_rb_tree() {
    wxg::rb_tree<int, int> rb;
    // insert_random_n(rb, 10, 0, 10);

    for (int i = 0; i < 10; i++) {
        rb.insert(i);
        // rb.traverse();
    }

    // for (int i = 0; i < 10; i++)
    // {
    //     rb.remove(i);
    //     rb.traverse();
    // }
    rb.traverse();
}

void test_btree() {
    wxg::btree<int, int, 5> bt;
    bt.traverse();

    for (auto &&i : {39, 22, 97, 41, 53}) bt.insert(i, i);
    bt.traverse();

    for (auto &&i : {13, 21, 40}) bt.insert(i, i);
    bt.traverse();

    for (auto &&i : {30, 27, 33, 36, 35, 34, 24, 29}) bt.insert(i, i);
    bt.traverse();

    bt.insert(26, 26);
    bt.traverse();

    for (auto &&i : {17, 28, 29, 31, 32, 23}) bt.insert(i, i);
    bt.traverse();

    cout << "remove 21" << endl;
    bt.remove(21);
    bt.traverse();

    cout << "remove 27" << endl;
    bt.remove(27);
    bt.traverse();

    cout << "remove 32" << endl;
    bt.remove(32);
    bt.traverse();

    cout << "remove 40" << endl;
    bt.remove(40);
    bt.traverse();

    cout << "remove 97" << endl;
    bt.remove(97);
    bt.traverse();
}

void test_bplus_tree() {
    wxg::bplust_tree<int, int, 5> bpt;

    for (auto &&i : {5, 8, 10, 15}) bpt.insert(i, i);
    bpt.traverse();

    for (auto &&i : {16, 17, 18}) bpt.insert(i, i);
    bpt.traverse();

    for (auto &&i : {6, 9, 18, 19}) bpt.insert(i, i);
    bpt.traverse();

    for (auto &&i : {20, 21, 22}) bpt.insert(i, i);
    bpt.traverse();

    for (auto &&i : {7}) bpt.insert(i, i);
    bpt.traverse();

    cout << "remove 22" << endl;
    bpt.remove(22);
    bpt.traverse();

    cout << "remove 15" << endl;
    bpt.remove(15);
    bpt.traverse();

    cout << "remove 7" << endl;
    bpt.remove(7);
    bpt.traverse();

    cout <<"traverse leaf:"<<endl;
    bpt.traverse_leaf();
}

int main(int argc, char const *argv[]) {
    // test_avl_tree();

    // test_rb_tree();

    test_btree();

    // test_bplus_tree();

    return 0;
}
