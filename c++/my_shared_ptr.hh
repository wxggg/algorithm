#include <iostream>

namespace wxg {

template <typename T>
class my_shared_ptr {
   private:
    T *m_ptr = nullptr;

    int *refcount = 0;

   public:
    explicit my_shared_ptr(T *p) : m_ptr(p), refcount(new int(1);) {}
    my_shared_ptr(my_shared_ptr &&p)
        : m_ptr(p.m_ptr), refcount(new int(1)) noexcept {
        p.m_ptr = nullptr;
    }

    my_shared_ptr(const my_shared_ptr &p) noexcept {
        m_ptr = p.m_ptr;
        refcount = p.refcount;
        ++*refcount;
    }

    my_shared_ptr &operator=(const my_shared_ptr &p) noexcept {
        if (m_ptr && --*refcount == 0) {
            delete m_ptr, delete refcount;
        }

        m_ptr = p.m_ptr;
        refcount = p.refcount;
        ++*refcount;
    }

    inline my_shared_ptr<T> &operator=(my_shared_ptr &&p) noexcept {
        return operator=(p);
    }

    explicit operator bool() const noexcept { return m_ptr; }

    ~my_shared_ptr() noexcept {
        if (--*refcount == 0 && m_ptr) {
            delete m_ptr;
            delete refcount;
        }
    }

    inline T &operator*() const noexcept { return *get(); }
    inline T *operator->() const noexcept { return get(); }

    inline T *get() const noexcept { return m_ptr; }

    inline void swap(my_shared_ptr &p) noexcept { std::swap(p.m_ptr, m_ptr); }
};
}  // namespace wxg
