
/**
 * Some metaprogramming for prefetching logic. The depth of possible 
 * prefetch stages is calculated recursively: be wary when using it, as
 * recursive definitions are easy to arrive to. 
 */


#ifdef GNU_PREFETCH

void lce_prefetch(const void * const loc) {
  __builtin_prefetch(loc,0,0);
}

template<typename T>
struct PrefetchDepth {
  enum {depth=0}
};

template<typename T, unsigned depth=0>
struct PrefetchPolicy {
  void prefetch(const T & tgt) {};
  void safe_prefetch(const T & tgt) {};
  enum {depth=0};
};

template<typename T, unsigned depth>
struct PrefetchPolicy<T*, depth> {
  /* We trust the compiler to eliminate unnecessarities: */
  void prefetch(const (T*) & tgt, size_t index=0) {
    PrefetchPolicy<T, depth-1>::prefetch(&(tgt[index]));
  }
  void safe_prefetch(const (T*) & tgt, size_t index=0) {
    if (tgt) PrefetchPolicy<T, depth-1>::safe_prefetch(&(tgt[index]));
  }
};

template<typename T>
struct PrefetchPolicy<T*,0> {
  void prefetch(const (T*) & tgt, size_t index=0) {
    lce_prefetch(&(tgt[index]));
  }
  void safe_prefetch(const (T*) & tgt, size_t index=0) {
    lce_prefetch(&(tgt[index]));
  }
};


#else 

void lce_prefetch(const void * const loc) {
  __builtin_prefetch(loc,0,0);
}

template<typename T, unsigned depth=0>
struct PrefetchPolicy {
  void prefetch(const T & tgt, size_t index=0) {};
};

#endif 

