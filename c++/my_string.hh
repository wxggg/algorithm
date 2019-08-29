#include <string.h>
#include <iostream>

namespace wxg {

class my_string {
   private:
    char *data_;

   public:
    my_string() : data_(new char[1]) { *data_ = '\0'; }
    my_string(const char *str) : data_(new char[strlen(str) + 1]) {
        strcpy(data_, str);
    }
    my_string(const my_string &rhs) : data_(new char[rhs.size() + 1]) {
        strcpy(data_, rhs.data_);
    }
    my_string(my_string &&rhs) : data_(rhs.data_) { rhs.data_ = nullptr; }
    ~my_string() { delete[] data_; }

    my_string &operator=(my_string rhs) {
        std::swap(data_, rhs.data_);
        return *this;
    }
    my_string &operator=(my_string &&rhs) {
        std::swap(data_, rhs.data_);
        return *this;
    }

    inline size_t size() const { return strlen(data_) + 1; }
    inline size_t length() const { return size(); }
    inline char *c_str() const { return data_; }
    inline void swap(my_string &rhs) { std::swap(data_, rhs.data_); }
};

}  // namespace wxg
