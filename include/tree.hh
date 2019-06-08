#include <functional>
#include <iostream>
#include <queue>
#include <stack>

using std::queue;

namespace wxg {

typedef bool __rb_tree_color_type;
const __rb_tree_color_type __red = false;
const __rb_tree_color_type __black = true;

struct __tree_node_base {
    typedef __rb_tree_color_type color_type;
    typedef __tree_node_base *base_ptr;

    color_type color;

    base_ptr parent = nullptr;
    base_ptr left = nullptr;
    base_ptr right = nullptr;

    static base_ptr minimum(base_ptr x) {
        while (x->left) x = x->left;
        return x;
    }

    static base_ptr maximum(base_ptr x) {
        while (x->right) x = x->right;
        return x;
    }
};

template <class Value>
struct __tree_node : public __tree_node_base {
    typedef __tree_node<Value> *link_type;
    Value value_field;
};

template <class Value>
inline __tree_node<Value> *__create_tree_node(const Value &v) {
    auto node = new __tree_node<Value>;
    node->value_field = v;
    return node;
}

template <typename T>
using Func = std::function<void(T &)>;

void __clear(__tree_node_base *p) {
    if (!p) return;
    __clear(p->left);
    __clear(p->right);
    delete p;
}

inline bool __is_black(const __tree_node_base *x) {
    return (!x || x->color == __black);
}

inline bool __is_red(const __tree_node_base *x) {
    return (x && x->color == __red);
}

inline void __rotate_left(__tree_node_base *x, __tree_node_base *&root) {
    auto y = x->right;

    x->right = y->left;
    if (y->left) y->left->parent = x;

    y->parent = x->parent;

    if (x == root)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

inline void __rotate_right(__tree_node_base *x, __tree_node_base *&root) {
    auto y = x->left;

    x->left = y->right;
    if (y->right) y->right->parent = x;

    y->parent = x->parent;

    if (x == root)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->right = x;
    x->parent = y;
}

inline __tree_node_base *__right_most(__tree_node_base *x) {
    while (x->right) x = x->right;
    return x;
}

inline __tree_node_base *__left_most(__tree_node_base *x) {
    while (x->left) x = x->left;
    return x;
}

// z is the value to be removed
// x is original position of the node to be moved to occupy z
inline __tree_node_base *__erase(__tree_node_base *z, __tree_node_base *&root,
                                 __tree_node_base *&x,
                                 __tree_node_base *&x_parent) {
    auto y = z;
    x = x_parent = nullptr;

    if (!y->left)
        x = y->right;
    else if (!y->right)
        x = y->left;
    else {
        y = __left_most(y->right);  // y
        x = y->right;
    }

    if (y == z) {
        x_parent = y->parent;
        if (x) x->parent = y->parent;
        if (z == root)
            root = x;
        else {
            if (z->parent->left == z)
                z->parent->left = x;
            else
                z->parent->right = x;
        }
    } else {  // y!=z so z->left and z->right must exists
        z->left->parent = y;
        y->left = z->left;
        if (y == z->right)
            x_parent = y;
        else {
            x_parent = y->parent;
            if (x) x->parent = y->parent;
            y->parent->left = x;  // y must be left to .... of z->right
            y->right = z->right;
            z->right->parent = y;
        }

        if (z == root)
            root = y;
        else if (z->parent->left == z)
            z->parent->left = y;
        else
            z->parent->right = y;

        y->parent = z->parent;
        std::swap(y->color, z->color);
    }

    delete z;

    return y;
}

// _x denotes the real position that be removed
inline __tree_node_base *__erase(__tree_node_base *z, __tree_node_base *&root) {
    __tree_node_base *x, *x_parent;
    return __erase(z, root, x, x_parent);
}

template <class Value>
inline __tree_node<Value> *__find(__tree_node<Value> *root, const Value &v) {
    auto x = root;
    while (x) {
        if (v == x->value_field)
            return x;
        else if (v < x->value_field)
            x = (__tree_node<Value> *)x->left;
        else
            x = (__tree_node<Value> *)x->right;
    }
    return nullptr;
}

template <class Value>
inline __tree_node_base *__insert(__tree_node<Value> *&root, const Value &v) {
    if (!root) {
        auto z = __create_tree_node(v);
        root = z;
        return z;
    }

    auto x = root;
    while (x) {
        if (v == x->value_field)
            return nullptr;
        else if (v < x->value_field) {
            if (!x->left) {
                x->left = __create_tree_node(v);
                x->left->parent = x;
                return x->left;
            } else
                x = (__tree_node<Value> *)x->left;
        } else {
            if (!x->right) {
                x->right = __create_tree_node(v);
                x->right->parent = x;
                return x->right;
            } else
                x = (__tree_node<Value> *)x->right;
        }
    }

    return nullptr;
}

// the will cuase performance problem
inline size_t __height(const __tree_node_base *x) {
    if (!x) return 0;
    return std::max(__height(x->left), __height(x->right)) + 1;
}

template <class Key, class Value>
class avl_tree {
   private:
    typedef __tree_node<Value> tree_node;
    typedef tree_node *link_type;
    typedef Value value_type;
    typedef __tree_node_base *base_ptr;

   private:
    size_t node_count;
    link_type header;

   private:
    link_type &root() const { return (link_type &)header->parent; }
    base_ptr &base_root() const { return header->parent; }
    link_type &leftmost() const { return (link_type &)header->left; }
    link_type &rightmost() const { return (link_type &)header->right; }

    static link_type &left(link_type x) { return (link_type &)(x->left); }
    static link_type &right(link_type x) { return (link_type &)(x->right); }
    static link_type &parent(link_type x) { return (link_type &)(x->parent); }

    static value_type value(const link_type x) { return x->value_field; }
    static value_type value(const base_ptr x) { return value((link_type)x); }

   public:
    avl_tree() : node_count(0) { init(); }
    ~avl_tree() {
        clear();
        delete header;
    }

    bool empty() { return !root(); }

    void erase(link_type z) { __erase(z, base_root()); }

    void clear() {
        __clear(root());
        root() = nullptr;
    }

    void insert(const Value &v) { insert_unique(v); }

    void insert_unique(const Value &v) {
        if (empty()) {
            auto z = __create_tree_node(v);
            root() = z;
            parent(z) = header;
            return;
        }

        auto z = __insert(root(), v);

        if (z) __rebalance(z);
    }

    void remove(const Value &v) {
        auto x = __find(root(), v);
        if (x) erase(x);
    }

    void traverse() {
        if (empty()) {
            std::cout << "_,\n";
            return;
        }
        queue<base_ptr> que;
        que.push(base_root());
        std::cout << value(que.front()) << ",";
        base_ptr p;
        while (!que.empty()) {
            p = que.front();
            que.pop();
            if (p->left) {
                std::cout << value(p->left) << ",";
                que.push(p->left);
            } else
                std::cout << "_,";

            if (p->right) {
                std::cout << value(p->right) << ",";
                que.push(p->right);
            } else
                std::cout << "_,";
        }
        std::cout << "\n";
    }

   private:
    void init() {
        header = new tree_node;
        root() = nullptr;
        leftmost() = header;
        rightmost() = header;
    }

    void __rebalance(base_ptr x) {
        while (x != header) {
            int balance = __height(x->left) - __height(x->right);
            if (balance > 1) {
                if (x->left->right) __rotate_left(x->left, base_root());
                __rotate_right(x, base_root());
            } else if (balance < -1) {
                if (x->right->left) __rotate_right(x->right, base_root());
                __rotate_left(x, base_root());
            } else {
                x = x->parent;
            }
        }
    }
};

template <class Key, class Value>
class rb_tree {
   private:
    typedef __tree_node<Value> tree_node;
    typedef tree_node *link_type;
    typedef Value value_type;
    typedef __tree_node_base *base_ptr;

   private:
    size_t node_count;
    link_type header;

   private:
    link_type &root() const { return (link_type &)header->parent; }
    base_ptr &base_root() const { return header->parent; }
    link_type &leftmost() const { return (link_type &)header->left; }
    link_type &rightmost() const { return (link_type &)header->right; }

    static link_type &left(link_type x) { return (link_type &)(x->left); }
    static link_type &right(link_type x) { return (link_type &)(x->right); }
    static link_type &parent(link_type x) { return (link_type &)(x->parent); }

    static value_type value(const link_type x) { return x->value_field; }
    static value_type value(const base_ptr x) { return value((link_type)x); }

   public:
    rb_tree() : node_count(0) { init(); }
    ~rb_tree() {
        clear();
        delete header;
    }

    bool empty() { return !root(); }

    void erase(link_type z) {
        base_ptr x = nullptr;
        base_ptr x_parent = nullptr;

        auto y = __erase(z, base_root(), x, x_parent);

        if (y->color == __black) __rebalance_for_erase(x, x_parent);
    }

    void clear() {
        __clear(root());
        root() = nullptr;
    }

    void insert(const Value &v) { insert_unique(v); }

    void insert_unique(const Value &v) {
        auto z = __insert(root(), v);

        if (z) __rebalance(z);
    }

    void remove(const Value &v) {
        auto x = __find(root(), v);
        if (x) erase(x);
    }

    void traverse() {
        if (empty()) {
            std::cout << "_,\n";
            return;
        }
        const char *color = "-+";  // -:red,+:black
        queue<base_ptr> que;
        que.push(base_root());
        int level = 0;
        base_ptr last = que.back();
        std::cout << color[que.front()->color] << value(que.front()) << ",";
        base_ptr p;
        while (!que.empty()) {
            p = que.front();
            que.pop();
            if (p->left) {
                std::cout << color[p->left->color] << value(p->left) << ",";
                que.push(p->left);
            } else
                std::cout << "_,";

            if (p->right) {
                std::cout << color[p->right->color] << value(p->right) << ",";
                que.push(p->right);
            } else
                std::cout << "_,";

            if (p == last) {
                level++;
                if (!que.empty()) last = que.back();
            }
        }
        std::cout << "\n";
        std::cout << "level=" << level << std::endl;
    }

   private:
    void init() {
        header = new tree_node;
        root() = nullptr;
        leftmost() = header;
        rightmost() = header;
    }

    void __rebalance(base_ptr x) {
        x->color = __red;
        bool isleft;
        base_ptr y;

        while (x != root() && __is_red(x->parent)) {
            isleft = (x->parent == x->parent->parent->left);

            if (isleft)
                y = x->parent->parent->right;
            else
                y = x->parent->parent->left;

            if (__is_red(y)) {
                x->parent->color = y->color = __black;
                x->parent->parent->color = __red;
                x = x->parent->parent;
            } else {
                if (isleft && x == x->parent->right)
                    __rotate_left(x->parent, base_root());
                else if (!isleft && x == x->parent->left)
                    __rotate_right(x->parent, base_root());
                else
                    x = x->parent;
                std::swap(x->color, x->parent->color);
                if (isleft)
                    __rotate_right(x->parent, base_root());
                else
                    __rotate_left(x->parent, base_root());
            }
        }
        root()->color = __black;
    }

    // pointer x might be null, there need a x_parent
    void __rebalance_for_erase(base_ptr x, base_ptr x_parent) {
        while (x != root() && __is_black(x)) {
            if (x == x_parent->left) {
                // because tree x needs one more black, x's brother w must exist
                auto w = x_parent->right;

                if (w->color == __red) {  // this make sure that w is black
                    std::swap(w->color, x_parent->color);
                    __rotate_left(x_parent, base_root());
                    w = x_parent->right;
                }

                // now w must be black
                if (__is_black(w->left) && __is_black(w->right)) {
                    w->color = __red;
                    x = x_parent;
                    x_parent = x_parent->parent;
                } else {  // there's at least one red on childs of w
                    if (__is_red(w->left)) {
                        std::swap(w->left->color, w->color);
                        __rotate_right(w, base_root());
                        w = x_parent->right;  // w is still black
                    }
                    // w extends x_parent's color,while x_parent turn black
                    std::swap(w->color, x_parent->color);
                    __rotate_left(x_parent, base_root());
                    // if w->right exists, then it must be red,
                    // and needs one more black
                    if (w->right) w->right->color = __black;
                    break;
                }
            } else {
                auto w = x_parent->left;

                if (w->color == __red) {
                    std::swap(w->color, x_parent->color);
                    __rotate_right(x_parent, base_root());
                    w = x_parent->left;
                }

                if (__is_black(w->left) && __is_black(w->right)) {
                    w->color = __red;
                    x = x_parent;
                    x_parent = x_parent->parent;
                } else {
                    if (__is_red(w->right)) {
                        std::swap(w->right->color, w->color);
                        __rotate_left(w, base_root());
                        w = x_parent->right;
                    }
                    std::swap(w->color, x_parent->color);
                    __rotate_right(x_parent, base_root());
                    if (w->left) w->left->color = __black;
                    break;
                }
            }
        }
        if (x) x->color = __black;
    }
};

}  // namespace wxg
