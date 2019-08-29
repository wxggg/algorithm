#include <iostream>

namespace wxg {

template <typename T>
class my_unique_ptr {
   private:
    T *m_ptr = nullptr;

   public:
    explicit my_unique_ptr(T *p) : m_ptr(p) {}
    my_unique_ptr(my_unique_ptr &&p) : m_ptr(p.m_ptr) noexcept {
        p.m_ptr = nullptr;
    }

    my_unique_ptr(const my_unique_ptr &) = delete;
    my_unique_ptr &operator=(const my_unique_ptr &) = delete;
    explicit operator bool() const noexcept { return m_ptr; }

    ~my_unique_ptr() {
        if (m_ptr) delete m_ptr;
    }

    inline my_unique_ptr<T> &operator=(my_unique_ptr &&p) noexcept {
        std::swap(*this, p);
        return *this;
    }

    inline T &operator*() const { return *get(); }
    inline T *operator->() const { return get(); }

    inline void reset(T *q = nullptr) noexcept {
        if (q != m_ptr) {
            if (m_ptr) delete m_ptr;
            m_ptr = q;
        }
    }

    inline T *release() noexcept {
        T *res = m_ptr;
        m_ptr = nullptr;
        return res;
    }

    inline T *get() const noexcept { return m_ptr; }

    inline void swap(my_unique_ptr &p) noexcept { std::swap(p.m_ptr, m_ptr); }
};
}  // namespace wxg
