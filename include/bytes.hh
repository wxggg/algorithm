#include <iostream>

inline size_t round_up(size_t x, size_t align) {
    return (((x) + (align - 1)) & ~(align - 1));
}

inline size_t index(size_t x, size_t align) {
    return (((x) + (align - 1)) / align - 1);
}