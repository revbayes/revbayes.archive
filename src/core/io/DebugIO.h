#ifndef DEBUG_IO_H
#define DEBUG_IO_H
template<typename T> void debug_vec(const char * prefix, const T * ptr, size_t n);
template<typename T> void debug_mat(const char * prefix, const T * ptr, size_t n, size_t m);

#if defined (REVBAYES_DEBUG_OUTPUT)
template<typename T>
inline void debug_vec(const char * pref, const T *x, size_t s) {
    std::cerr << pref << " at " << (long) pref << " = [";
    for (size_t i = 0; i < s; ++i) {
        if (i != 0) {
            std::cerr << ", ";
        }
        std::cerr << x[i];
    }
    std::cerr << "]\n";
}
template<typename T>
inline void debug_mat(const char * pref, const T *x, size_t n, size_t m) {
    std::cerr << pref << " at " << (long) pref << " =  [";
    for (size_t j = 0; j < n; ++j) {
        if (j != 0) {
            std::cerr << ",";
        }
        std::cerr << "\n  ";
        for (size_t i = 0; i < m; ++i) {
            if (i != 0) {
                std::cerr << ", ";
            }
            std::cerr << x[i];
        }
    }
    std::cerr << "]\n";
}
#else
template<typename T>
inline void debug_vec(const char * , const T *, size_t){
}
template<typename T>
inline void debug_mat(const char * , const T *, size_t , size_t ) {
}

#endif
#endif
