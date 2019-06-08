#include <algorithm>
#include <fstream>
#include <map>
#include <queue>
#include <set>
#include <string>

namespace wxg {

struct huffman_node {
    size_t weight;
    struct huffman_node *left = nullptr;
    struct huffman_node *right = nullptr;

    inline bool isleaf() { return !left && !right; }
};

template <class Value>
struct huffman_leaf_node : public huffman_node {
    Value value;
};

template <class Value>
class huffman_tree {
   private:
    typedef huffman_leaf_node<Value> leaf_t;
    typedef huffman_node node_t;

    std::map<Value, size_t> freqs;
    std::map<Value, std::string> encodes;

    node_t *root = nullptr;

   public:
    huffman_tree() {}
    ~huffman_tree() {}

    bool empty() { return !root; }

    void insert(const Value &v, size_t n) { freqs[v] = n; }

    void build() {
        auto compare = [](node_t *x, node_t *y) {
            return x->weight < y->weight;
        };
        auto huffmanSet = std::multiset<node_t *, decltype(compare)>(compare);

        for (const auto &kv : freqs) {
            leaf_t *leaf = new leaf_t;
            leaf->value = kv.first;
            leaf->weight = kv.second;
            huffmanSet.insert((node_t *)leaf);
        }

        while (huffmanSet.size() > 1) {
            node_t *node1 = *huffmanSet.begin();
            huffmanSet.erase(huffmanSet.begin());
            node_t *node2 = *huffmanSet.begin();
            huffmanSet.erase(huffmanSet.begin());

            node_t *node = new node_t;
            node->left = node1;
            node->right = node2;
            node->weight = node1->weight + node2->weight;

            huffmanSet.insert(node);
        }

        root = *huffmanSet.begin();
    }

    void encode(node_t *node, std::string code) {
        if (node->isleaf())
            encodes[((leaf_t *)node)->value] = code;
        else {
            encode(node->left, code + "0");
            encode(node->right, code + "1");
        }
    }

    void encode() {
        if (empty()) return;
        encode(root, "");

        for (const auto &kv : encodes) {
            std::cout << kv.first << ":" << kv.second << std::endl;
        }
    }

    void traverse() {
        if (empty()) {
            std::cout << "[,],\n";
            return;
        }
        std::queue<node_t *> que;
        que.push(root);
        node_t *p;
        while (!que.empty()) {
            p = que.front();
            que.pop();
            if (p->isleaf())
                std::cout << "[" << ((leaf_t *)p)->value << "," << p->weight
                          << "],";
            else {
                std::cout << "[," << p->weight << "],";
                que.push(p->left);
                que.push(p->right);
            }
        }
        std::cout << "\n";
    }
};

}  // namespace wxg
