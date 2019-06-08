#include <iostream>

#include "include/huffman_tree.hh"

void test_huffman() {
    wxg::huffman_tree<char> huffTree;

    huffTree.insert('a', 12);
    huffTree.insert('b', 3);
    huffTree.insert('c', 4);
    huffTree.insert('d', 5);
    huffTree.insert('e', 8);

    huffTree.build();

    huffTree.traverse();

    huffTree.encode();
}

int main(int argc, char const *argv[]) {
    test_huffman();
    return 0;
}
