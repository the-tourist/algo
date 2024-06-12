template <typename T>
class FenwickTree {
 public:
  vector<T> fenw;
  int n;
  int pw;

  FenwickTree(int _n) : n(_n) {
    fenw.resize(n);
    pw = bit_floor(uint32_t(n));
  }

  void Modify(int x, T v) {
    assert(0 <= x && x < n);
    while (x < n) {
      fenw[x] += v;
      x |= x + 1;
    }
  }

  T Get(int x) {
    assert(0 <= x && x <= n);
    T v{};
    while (x > 0) {
      v += fenw[x - 1];
      x &= x - 1;
    }
    return v;
  }

  // Returns the length of the longest prefix with sum <= c
  int MaxPrefix(T c) {
    T v{};
    int at = 0;
    for (int len = pw; len > 0; len >>= 1) {
      if (at + len <= n) {
        auto nv = v;
        nv += fenw[at + len - 1];
        if (!(c < nv)) {
          v = nv;
          at += len;
        }
      }
    }
    assert(0 <= at && at <= n);
    return at;
  }
};
