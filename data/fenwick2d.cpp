template <typename T>
class FenwickTree2D {
 public:
  vector<vector<T>> fenw;
  int n, m;

  FenwickTree2D() : n(0), m(0) {}
  FenwickTree2D(int n_, int m_) : n(n_), m(m_) {
    fenw.resize(n);
    for (int i = 0; i < n; i++) {
      fenw[i].resize(m);
    }
  }

  void Modify(int i, int j, T v) {
    assert(0 <= i && i < n && 0 <= j && j < m);
    int x = i;
    while (x < n) {
      int y = j;
      while (y < m) {
        fenw[x][y] += v;
        y |= y + 1;
      }
      x |= x + 1;
    }
  }

  T Query(int i, int j) {
    assert(0 <= i && i <= n && 0 <= j && j <= m);
    T v{};
    int x = i;
    while (x > 0) {
      int y = j;
      while (y > 0) {
        v += fenw[x - 1][y - 1];
        y &= y - 1;
      }
      x &= x - 1;
    }
    return v;
  }
};
