#include "heap.hh"

namespace wxg {

template <typename T>
class priority_queue {
   private:
    std::vector<T> c;

   public:
    priority_queue() {}
    ~priority_queue() {}

    bool empty() const { return c.empty(); }
    const T &top() const { return c.front(); }

    void push(const T &x) { heap::push(c, x); }

    void push_back(const T &x) { push(x); }

    void pop() { heap::pop(c); }
};

}  // namespace wxg
