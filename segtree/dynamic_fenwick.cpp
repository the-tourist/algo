template <typename T>
class DynamicFenwickTree {
 public:
  HashMap<int, T> fenw;
  int n;
  int pw;

  DynamicFenwickTree() : n(0) {}
  DynamicFenwickTree(int n_) : n(n_) {
    pw = bit_floor(unsigned(n));
  }

  void Modify(int x, T v) {
    assert(0 <= x && x < n);
    while (x < n) {
      fenw[x] += v;
      x |= x + 1;
    }
  }

  T Query(int x) {
    assert(0 <= x && x <= n);
    T v{};
    while (x > 0) {
      auto it = fenw.find(x - 1);
      if (it != fenw.end()) {
        v += it->second;
      }
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
        auto it = fenw.find(at + len - 1);
        if (it != fenw.end()) {
          nv += it->second;
        }
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
