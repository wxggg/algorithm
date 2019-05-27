#include <vector>
#include "hash.hh"
#include "vector.hh"

namespace wxg {

using std::vector;

template <class Value>
struct __hashtable_node {
    __hashtable_node *next = nullptr;
    Value val;
    __hashtable_node(const Value &v) : val(v) {}
};

template <class Value>
__hashtable_node<Value> *create_node(const Value &v) {
    auto p = new __hashtable_node<Value>(v);
    p->next = nullptr;
    return p;
}

static const vector<unsigned long> __prime_list = {
    53ul,         97ul,         193ul,       389ul,       769ul,
    1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
    49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
    1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
    50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
    1610612741ul, 3221225473ul, 4294967291ul};

inline unsigned long __next_prime(unsigned long __n) {
    const auto pos = lower_bound(__prime_list, __n);
    return pos == __prime_list.size() ? __prime_list[__prime_list.size() - 1]
                                      : __prime_list[pos];
}

template <class Value, class Key>
class hashtable {
   private:
    typedef __hashtable_node<Value> node;

    vector<node *> buckets;
    size_t num_elements;

   public:
    hashtable() { initialize_buckets(50); }
    hashtable(size_t n) { initialize_buckets(n); }
    ~hashtable() {}

    size_t bucket_count() const { return buckets.size(); }
    size_t max_bucket_count() const { return *(__prime_list.end() - 1); }
    size_t next_size(size_t n) const { return __next_prime(n); }

    void initialize_buckets(size_t n) {
        const size_t n_buckets = next_size(n);
        buckets.reserve(n_buckets);
        buckets.insert(buckets.end(), n_buckets, nullptr);
        num_elements = 0;
    }

    void clear() {
        for (size_t i = 0; i < buckets.size(); ++i) {
            node *p = buckets[i];
            while (p) {
                auto next = p->next;
                delete p;
                p = next;
            }
            buckets[i] = nullptr;
        }
        num_elements = 0;
    }

    void copy_from(hashtable<Value, Key> &ht) {
        buckets.clear();
        buckets.reserve(ht.buckets.size());
        buckets.insert(buckets.end(), ht.buckets.size(), nullptr);

        for (size_t i = 0; i < buckets.size(); ++i) {
            if (const node *p = ht.buckets[i]) {
                node *copy = create_node(p->val);
                buckets[i] = copy;
                node *next = p->next;
                while (next) {
                    copy->next = create_node(next->val);
                    copy = copy->next;
                    next = next->next;
                }
            }
        }
        num_elements = ht.num_elements;
    }

    void insert(const Value &v) { insert_unique(v); }

    bool insert_unique(const Value &v) {
        resize(num_elements + 1);
        return insert_unique_noresize(v);
    }

    void resize(size_t num_elements_hint) {
        const size_t old_n = buckets.size();
        if (num_elements_hint > old_n) {
            const size_t n = next_size(num_elements_hint);
            vector<node *> tmp(n, nullptr);
            for (size_t i = 0; i < old_n; ++i) {
                while (buckets[i]) {
                    auto p = buckets[i];
                    buckets[i] = buckets[i]->next;

                    size_t new_bucket = bkt_num(p->val, n);
                    p->next = tmp[new_bucket];
                    tmp[new_bucket] = p;
                }
            }
            buckets.swap(tmp);
        }
    }

    bool insert_unique_noresize(const Value &v) {
        const size_t n = bkt_num(v);
        auto p = buckets[n];

        while (p) {
            if (p->val == v) return false;
            p = p->next;
        }

        node *tmp = create_node(v);
        tmp->next = buckets[n];
        buckets[n] = tmp;

        ++num_elements;

        return true;
    }

    size_t erase(const Key &key) {
        const size_t n = bkt_num(key);
        size_t count = 0;
        node *first = buckets[n];
        if (first) {
            node *p = first;
            node *next = p->next;
            while (next) {
                if (next->val == key) {
                    p->next = next->next;
                    delete next;
                    next = p->next;
                    ++count;
                    --num_elements;
                } else {
                    p = next;
                    next = p->next;
                }
            }
            if (first->val == key) {
                buckets[n] = first->next;
                delete first;
                ++count;
                --num_elements;
            }
        }
        return count;
    }

    void traverse() {
        for (size_t i = 0; i < buckets.size(); i++) {
            if (buckets[i]) {
                node *p = buckets[i];
                while (p) {
                    std::cout << p->val << ",";
                    p = p->next;
                }
                std::cout << " # ";
            }
        }
        std::cout << std::endl;
    }

   private:
    size_t bkt_num(const Value &v, size_t n) const { return bkt_num_key(v, n); }

    size_t bkt_num(const Value &v) const { return bkt_num_key(v); }

    size_t bkt_num_key(const Key &key) const {
        return bkt_num_key(key, buckets.size());
    }

    size_t bkt_num_key(const Key &key, size_t n) const { return hash(key) % n; }
};

}  // namespace wxg
