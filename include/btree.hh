#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>

using std::binary_search;
using std::lower_bound;
using std::upper_bound;

namespace wxg {

template <class Key, size_t M>
struct __btree_node_base {
    Key key[M];                                   // at most M-1, [0, M)
    __btree_node_base* child[M + 1] = {nullptr};  // at most M, [0, M+1)
    __btree_node_base* parent = nullptr;
    size_t n = 0;

    int insert_key(const Key& k) {
        auto i = lower_bound(key, key + n, k) - key;
        std::copy_backward(key + i, key + n, key + n + 1);
        key[i] = k;
        return i;
    }

    void insert_child(int i, __btree_node_base* p) {
        std::copy_backward(child + i, child + n + 1, child + n + 2);
        child[i] = p;
        if (p) p->parent = this;
    }

    void split_key(__btree_node_base* p) {
        int i = M / 2;
        std::copy(key + i + 1, key + n, p->key);
    }

    void split_child(__btree_node_base* p) {
        int i = M / 2;
        std::copy(child + i + 1, child + n + 1, p->child);
        if (!isleaf())
            for (int j = i + 1; j <= n; j++) child[j]->parent = p;
    }

    void merge_child(__btree_node_base* p) {
        if (!p->isleaf())
            for (int i = 0; i <= p->n; i++) p->child[i]->parent = this;
        std::copy(p->child, p->child + p->n + 1, child + n + 1);
    }

    void remove_key(int i) { std::copy(key + i + 1, key + n, key + i); }

    void remove_child(int i) {
        std::copy(child + i + 1, child + n + 1, child + i);
    }

    bool isleaf() { return !child[0]; }
};

template <class Key, class Value, size_t M>
struct __btree_node : public __btree_node_base<Key, M> {
    Value value[M];

    int insert_value(int i, const Value& v) {
        std::copy_backward(value + i, value + this->n, value + this->n + 1);
        value[i] = v;
        return i;
    }

    int insert(const Key& k, const Value& v) {
        return insert_value(this->insert_key(k), v);
    }

    void split_value(__btree_node* p) {
        int i = M / 2;
        std::copy(value + i + 1, value + this->n, p->value);
    }

    void split(__btree_node* p) {
        this->split_key(p);
        this->split_child(p);
        split_value(p);
    }

    void remove_value(int i) {
        std::copy(value + i + 1, value + this->n, value + i);
    }
};

template <class Key, class Value, size_t M>
struct __bplus_leaf_node : public __btree_node<Key, Value, M> {
    __bplus_leaf_node* next = nullptr;
};

template <class Key, class Value, size_t M = 20>
class btree {
   private:
    typedef __btree_node<Key, Value, M> tree_node;
    typedef tree_node* link_type;
    typedef Value value_type;
    typedef Key key_type;

   private:
    size_t height = 0;
    link_type header;

    const static int BORDER = std::ceil(1.0 * M / 2) - 1;

    link_type& root() const { return (link_type&)header->parent; }

   public:
    btree() {
        header = new tree_node;
        root() = nullptr;
    }
    ~btree() {
        clear();
        delete header;
    }

    bool empty() { return !root(); }

    void clear() {}

    link_type search_key(const Key& k) const {
        link_type p = root();
        for (int h = 1; h < height; h++) {
            size_t i = lower_bound(p->key, p->key + p->n, k) - p->key;
            if (i != p->n && p->key[i] == k) return p;  // found

            p = (link_type)p->child[i];
        }
        return p;
    }

    void split(link_type p) {
        int i = M / 2;

        auto brother = new tree_node;
        auto parent = (link_type)p->parent;
        int index = parent->insert(p->key[i], p->value[i]);
        parent->insert_child(index + 1, brother);
        parent->n++;

        p->split(brother);
        brother->n = M - i - 1;
        p->n = i;
    }

    int insert(const Key& k, const Value& v) {
        if (empty()) {
            root() = new tree_node;
            root()->insert(k, v);
            root()->n = 1;
            height = 1;
            return 0;
        }
        auto p = search_key(k);
        if (binary_search(p->key, p->key + p->n, k)) return 1;

        p->insert(k, v);
        p->n++;

        while (p != root() && p->n == M) {
            split(p);
            p = (link_type)p->parent;
        }

        if (p->n == M) {
            root() = new tree_node;
            root()->insert_child(0, p);
            split(p);

            height++;
        }
        return 0;
    }

    int update(const Key& k, const Value& v) {
        if (empty()) return -1;
        auto p = search_key(k);
        int i = lower_bound(p->key, p->key + p->n, k) - p->key;
        if (i == p->n) return -1;

        p->key[i] = v;
        return 0;
    }

    int borrow(link_type p, int i /* parent key pos*/, link_type brother,
               bool from_left) {
        auto parent = (link_type)p->parent;
        int pos = p->insert(parent->key[i], parent->value[i]);

        int j = 0;
        if (from_left) {
            j = brother->n - 1;
            p->insert_child(pos, brother->child[j + 1]);
            brother->remove_child(j + 1);
        } else {
            p->insert_child(pos, brother->child[0]);
            brother->remove_child(0);
        }
        p->n++;

        parent->key[i] = brother->key[j];
        parent->value[i] = brother->value[j];

        brother->remove_key(j);
        brother->remove_value(j);
        brother->n--;
        return 0;
    }

    /** merge right to left */
    int merge(link_type left, int i /* parent key pos*/, link_type right) {
        auto parent = (link_type)left->parent;

        left->key[left->n] = parent->key[i];
        std::copy(right->key, right->key + right->n, left->key + left->n + 1);

        left->value[left->n] = parent->value[i];
        std::copy(right->value, right->value + right->n,
                  left->value + left->n + 1);

        left->merge_child(right);

        left->n += right->n + 1;

        parent->remove_key(i);
        parent->remove_value(i);
        parent->remove_child(i + 1);  // remove right
        parent->n--;

        return 0;
    }

    int remove(const Key& k) {
        if (empty()) return -1;
        auto p = search_key(k);
        int i = lower_bound(p->key, p->key + p->n, k) - p->key;
        if (i == p->n) return -1;

        if (!p->isleaf()) {
            auto pnext = (link_type)p->child[i + 1];  // find next key
            while (!pnext->isleaf()) pnext = (link_type)pnext->child[0];
            p->key[i] = pnext->key[0];
            p->value[i] = pnext->value[0];
            p = pnext;
            i = 0;
        }

        // now p is sure to be leaf
        p->remove_key(i);
        p->remove_value(i);
        p->n--;

        // values is too little
        while (p != root() && p->n < BORDER) {
            auto parent = (link_type)p->parent;
            i = upper_bound(parent->key, parent->key + parent->n, p->key[0]) -
                parent->key;

            link_type brother;
            if (i > 0) {  // left exist
                brother = (link_type)parent->child[i - 1];
                if (brother->n > BORDER) return borrow(p, i - 1, brother, true);
                merge(brother, i - 1, p);
                delete p;
                p = brother;
            } else {  // right exist
                brother = (link_type)parent->child[i + 1];
                if (brother->n > BORDER) return borrow(p, i, brother, false);
                merge(p, i, brother);
                delete brother;
            }
            if (parent == root() && parent->n == 0) {
                delete root();
                root() = p;
                height--;
                return 0;
            }
            p = parent;
        }
        return 0;
    }

    int print_node(link_type p) {
        std::cout << "[";
        if (p)
            for (int i = 0; i < p->n; i++) std::cout << p->key[i] << ",";
        std::cout << "]-";
        return 0;
    }

    void traverse() {
        if (empty()) {
            std::cout << "[],\n";
            return;
        }
        std::queue<link_type> que;
        std::queue<link_type> tmp;
        que.push(root());
        link_type p;
        while (true) {
            while (!que.empty()) {
                p = que.front();
                print_node(p);
                que.pop();
                for (int i = 0; i < p->n + 1; i++) {
                    auto child = (link_type)p->child[i];
                    if (child) tmp.push(child);
                }
            }
            if (tmp.empty()) break;
            que.swap(tmp);
        }
        std::cout << std::endl;
    }
};

template <class Key, class Value, size_t M = 20>
class bplust_tree {
   private:
    typedef __btree_node_base<Key, M> internal_node;
    typedef __bplus_leaf_node<Key, Value, M> leaf_node;

    typedef Value value_type;
    typedef Key key_type;

   private:
    size_t height = 0;
    internal_node* header;
    leaf_node* first = nullptr;

    const static int BORDER = std::ceil(1.0 * M / 2) - 1;

    internal_node*& root() { return (internal_node*&)header->parent; }

   public:
    bplust_tree() {
        header = new internal_node;
        root() = nullptr;
    }

    ~bplust_tree() {
        clear();
        delete header;
    }

    bool empty() { return !root(); }

    void clear() {}

    leaf_node* search_key(const Key& k) {
        auto p = root();
        while (!p->isleaf()) {
            int i = upper_bound(p->key, p->key + p->n, k) - p->key;
            p = p->child[i];
        }
        return (leaf_node*)p;
    }

    void split_internal(internal_node* p) {
        int i = M / 2;
        auto brother = new internal_node;
        auto parent = (internal_node*)p->parent;
        int index = parent->insert_key(p->key[i]);
        parent->insert_child(index + 1, brother);
        parent->n++;

        p->split_child(brother);
        p->split_key(brother);
        brother->n = M - i - 1;
        p->n = i;
    }

    void split_leaf(leaf_node* p) {
        int i = M / 2;
        Key k = p->key[i];
        Value v = p->value[i];

        auto brother = new leaf_node;
        auto parent = (internal_node*)p->parent;
        int index = parent->insert_key(p->key[i]);
        parent->insert_child(index + 1, brother);
        parent->n++;

        p->split(brother);
        brother->n = M - i - 1;
        p->n = i;

        brother->insert(k, v);  // b+tree: k,v should still be in leaf
        brother->n++;

        brother->next = p->next;
        p->next = brother;
    }

    int insert(const Key& k, const Value& v) {
        if (empty()) {
            first = new leaf_node;
            first->insert(k, v);
            first->n = 1;
            root() = first;
            height = 1;
            return 0;
        }

        auto p = search_key(k);
        if (binary_search(p->key, p->key + p->n, k)) return 1;

        p->insert(k, v);
        p->n++;

        if (p->n < M) return 0;

        if (p == root()) {  // root is leaf
            root() = new internal_node;
            root()->insert_child(0, p);
            split_leaf(p);
            height++;
            return 0;
        }

        // p is not root
        split_leaf(p);
        internal_node* pi = p->parent;

        while (pi != root() && pi->n == M) {
            split_internal(pi);
            pi = pi->parent;
        }

        if (pi->n == M) {
            root() = new internal_node;
            root()->insert_child(0, pi);
            split_internal(pi);
            height++;
        }

        return 0;
    }

    int update(const Key& k, const Value& v) {
        if (empty()) return -1;
        auto p = search_key(k);
        int i = lower_bound(p->key, p->key + p->n, k) - p->key;
        if (i == p->n) return -1;

        p->key[i] = v;
        return 0;
    }

    int borrow_leaf(leaf_node* p, int i /*parent key pos*/, leaf_node* brother,
                    bool from_left) {
        internal_node* parent = p->parent;

        int j = 0;
        if (from_left) {
            j = brother->n - 1;
            parent->key[i] = brother->key[j];
        } else
            parent->key[i] = brother->key[1];

        p->insert(brother->key[j], brother->value[j]);
        p->n++;

        brother->remove_key(j);
        brother->remove_value(j);
        brother->n--;
        return 0;
    }

    int merge_leaf(leaf_node* left, int i /*parent key pos */,
                   leaf_node* right) {
        internal_node* parent = left->parent;

        std::copy(right->key, right->key + right->n, left->key + left->n);
        std::copy(right->value, right->value + right->n, left->value + left->n);

        left->n += right->n;

        parent->remove_key(i);
        parent->remove_child(i + 1);  // remove right;
        parent->n--;

        left->next = right->next;
        return 0;
    }

    int merge_internal(internal_node* left, int i /*parent key pos*/,
                       internal_node* right) {
        auto parent = left->parent;

        left->key[left->n] = parent->key[i];
        std::copy(right->key, right->key + right->n, left->key + left->n + 1);

        left->merge_child(right);
        left->n += right->n + 1;

        parent->remove_key(i);
        parent->remove_child(i + 1);  // remove right
        parent->n--;

        return 0;
    }

    int borrow_internal(internal_node* p, int i /*parent key pos*/,
                        internal_node* brother, bool from_left) {
        internal_node* parent = p->parent;
        int pos = p->insert_key(parent->key[i]);

        int j = 0;
        if (from_left) {
            j = brother->n - 1;
            p->insert_child(pos, brother->child[j + 1]);
            brother->remove_child(j + 1);
        } else {
            p->insert_child(pos, brother->child[0]);
            brother->remove_child(0);
        }
        p->n++;

        parent->key[i] = brother->key[j];

        brother->remove_key(j);
        return 0;
    }

    int remove(const Key& k) {
        if (empty()) return -1;
        auto p = search_key(k);
        int i = lower_bound(p->key, p->key + p->n, k) - p->key;
        if (i == p->n) return -1;

        p->remove_key(i);
        p->remove_value(i);
        p->n--;

        if (p == root() || p->n >= BORDER) return 0;

        // borrow or merge from leaf brother
        internal_node* parent = p->parent;
        i = upper_bound(parent->key, parent->key + parent->n, p->key[0]) -
            parent->key;

        leaf_node* leafbro;
        if (i > 0) {  // left exist
            leafbro = (leaf_node*)parent->child[i - 1];
            if (leafbro->n > BORDER)
                return borrow_leaf(p, i - 1, leafbro, true);
            merge_leaf(leafbro, i - 1, p);
            delete p;
            p = leafbro;
        } else {  // right exist
            leafbro = (leaf_node*)parent->child[i + 1];
            if (leafbro->n > BORDER) return borrow_leaf(p, i, leafbro, false);
            merge_leaf(p, i, leafbro);
            delete leafbro;
        }

        internal_node* pi = parent;

        // internal node need borrow or merge
        while (pi != root() && pi->n < BORDER) {
            parent = pi->parent;
            i = upper_bound(parent->key, parent->key + parent->n, pi->key[0]) -
                parent->key;

            internal_node* brother;
            if (i > 0) {
                brother = parent->child[i - 1];
                if (brother->n > BORDER)
                    return borrow_internal(pi, i - 1, brother, true);
                merge_internal(brother, i - 1, pi);
                delete pi;
                pi = brother;
            } else {
                brother = parent->child[i + 1];
                if (brother->n > BORDER)
                    return borrow_internal(pi, i, brother, false);
                merge_internal(pi, i, brother);
                delete brother;
            }

            if (parent == root() && parent->n == 0) {
                delete root();
                root() = pi;
                height--;
                return 0;
            }
            pi = parent;
        }

        return 0;
    }

    int print_node(internal_node* p) {
        std::cout << "[";
        if (p)
            for (int i = 0; i < p->n; i++) std::cout << p->key[i] << ",";
        std::cout << "]-";
        return 0;
    }

    void traverse() {
        if (empty()) {
            std::cout << "[],\n";
            return;
        }
        std::queue<internal_node*> que;
        std::queue<internal_node*> tmp;
        que.push(root());
        internal_node* p;
        while (true) {
            while (!que.empty()) {
                p = que.front();
                print_node(p);
                que.pop();
                for (int i = 0; i < p->n + 1; i++) {
                    auto child = (internal_node*)p->child[i];
                    if (child) tmp.push(child);
                }
            }
            if (tmp.empty()) break;
            que.swap(tmp);
        }
        std::cout << std::endl;
    }

    void traverse_leaf() {
        if (empty()) {
            std::cout << "[],\n";
            return;
        }
        auto p = first;
        for (auto p = first; p; p = p->next) print_node(p);
        std::cout << std::endl;
    }
};

}  // namespace wxg
