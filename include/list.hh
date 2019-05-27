#include <functional>
#include <iostream>

namespace wxg {

template <typename T>
struct list_node {
    T value;
    struct list_node *prev;
    struct list_node *next;
};

template <typename T>
class list {
   private:
    list_node<T> *__head = nullptr;

   public:
    list() {
        __head = new list_node<T>;
        __head->prev = nullptr;

        auto p = new list_node<T>;
        p->prev = p->next = p;

        __head->next = p;
    }
    ~list() {
        clear();
        delete head();
        delete __head;
    }

    inline list_node<T> *head() { return __head->next; }
    inline list_node<T> *begin() { return head()->next; }
    inline list_node<T> *end() { return head(); }
    inline bool empty() { return begin() == end(); }

    void swap(list &x) { std::swap(__head, x.__head); }

    list_node<T> *erase(list_node<T> *p) {
        auto prev = p->prev;
        auto next = p->next;
        prev->next = next;
        next->prev = prev;
        delete p;
        return next;
    }

    // insert p prior to pos
    void insert(list_node<T> *pos, list_node<T> *p) {
        auto prev = pos->prev;
        prev->next = p;
        p->prev = prev;
        p->next = pos;
        pos->prev = p;
    }

    void push_front(const T &value) {
        auto p = new list_node<T>;
        p->value = value;
        insert(begin(), p);
    }

    void push_back(const T &value) {
        auto p = new list_node<T>;
        p->value = value;
        insert(end(), p);
    }

    void pop_front() { erase(begin()); }
    void pop_back() {
        auto tmp = end()->prev;
        erase(tmp);
    }

    void clear() {
        auto p = begin();
        while (p != end()) {
            auto tmp = p;
            p = p->next;
            delete (tmp);
        }
        head()->prev = head();
        head()->next = head();
    }

    void remove(const T &value) {
        auto p = begin();
        while (p != end()) {
            auto next = p->next;
            if (value == p->value) erase(p);
            p = next;
        }
    }

    // remove near and same values to unique
    void unique() {
        auto p = begin();
        if (p == end()) return;
        auto next = p->next;
        while (next != end()) {
            if (next->value == p->value)
                erase(next);
            else
                p = next;
            next = p->next;
        }
    }

    // move [first, last) prior to pos
    void transfer(list_node<T> *pos, list_node<T> *first, list_node<T> *last) {
        auto first_prev = first->prev;
        auto last_prev = last->prev;
        auto pos_prev = pos->prev;

        first_prev->next = last;
        last->prev = first_prev;

        last_prev->next = pos;
        pos->prev = last_prev;

        pos_prev->next = first;
        first->prev = pos_prev;
    }

    // splice x to prior pos
    void splice(list_node<T> *pos, list &x) {
        if (!x.empty()) transfer(pos, x.begin(), x.end());
    }

    // splice node p to prior pos, i & p can belong to same list
    void splice(list_node<T> *pos, list &, list_node<T> *p) {
        auto next = p->next;
        if (next == p || next == pos) return;
        transfer(pos, p, next);
    }

    // splice [first, last) to prior pos
    // pos & [first, last) can belong to same list
    // pos must not in [first, last)
    void splice(list_node<T> *pos, list &, list_node<T> *first,
                list_node<T> *last) {
        if (first != last) transfer(pos, first, last);
    }

    void merge(list &x) {
        auto p1 = begin();
        auto p2 = x.begin();
        while (p1 != end() && p2 != x.end()) {
            if (p2->value < p1->value) {
                auto next = p2->next;
                transfer(p1, p2, next);
                p2 = next;
            } else
                p1 = p1->next;
        }
        if (p2 != x.end()) transfer(end(), p2, x.end());
    }

    void reverse() {
        if (empty() || begin()->next == end()) return;
        auto p = begin()->next;
        while (p != end()) {
            auto next = p->next;
            transfer(begin(), p, next);
            p = next;
        }
    }

    void sort() {
        if (empty() || begin()->next == end()) return;

        list<T> carry;
        list<T> counter[64];
        int fill = 0;
        while (!empty()) {
            carry.splice(carry.begin(), *this, begin());
            int i = 0;
            while (i < fill && !counter[i].empty()) {
                counter[i].merge(carry);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if (i == fill) ++fill;
        }

        for (int i = 1; i < fill; ++i) counter[i].merge(counter[i - 1]);

        swap(counter[fill - 1]);
    }

    void insert_sort() {
        using std::cout;
        if (empty() || begin()->next == end()) return;

        auto p = begin()->next;
        while (p != end()) {
            auto q = p;
            while (q->prev != begin() && q->prev->value > p->value) q = q->prev;
            if (q == p)
                p = p->next;
            else {
                auto next = p->next;
                splice(q, *this, p);
                p = next;
            }
        }
    }

    void tranverse(std::function<void(T &)> &&f) {
        auto p = begin();
        while (p != end()) {
            f(p->value);
            p = p->next;
        }
    }
};

}  // namespace wxg
