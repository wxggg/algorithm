#include <iostream>

namespace wxg {

inline static size_t __hash_string(const char *s) {
    unsigned long h = 0;
    for (; *s; ++s) h = 5 * h + *s;

    return size_t(h);
}

inline size_t hash(char *s) { return __hash_string(s); }
inline size_t hash(const char *s) { return __hash_string(s); }
inline size_t hash(char x) { return x; }
inline size_t hash(unsigned char x) { return x; }
inline size_t hash(signed char x) { return (unsigned char)x; }
inline size_t hash(short x) { return x; }
inline size_t hash(unsigned short x) { return x; }
inline size_t hash(int x) { return x; }
inline size_t hash(unsigned int x) { return x; }
inline size_t hash(long x) { return x; }
inline size_t hash(unsigned long x) { return x; }

}  // namespace wxg
