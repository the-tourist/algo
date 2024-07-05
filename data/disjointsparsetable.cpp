template <typename T, typename F>
class DisjointSparseTable {
 public:
  int n;
  vector<vector<T>> mat;
  F func;

  DisjointSparseTable(const vector<T>& a, const F& f) : n(int(a.size())), func(f) {
    mat.push_back(a);
    for (int p = 1; (1 << p) < n; p++) {
      mat.emplace_back(n);
      for (int mid = 1 << p; mid < n; mid += 1 << (p + 1)) {
        mat[p][mid - 1] = a[mid - 1];
        for (int j = mid - 2; j >= mid - (1 << p); j--) {
          mat[p][j] = func(a[j], mat[p][j + 1]);
        }
        mat[p][mid] = a[mid];
        for (int j = mid + 1; j < min(n, mid + (1 << p)); j++) {
          mat[p][j] = func(mat[p][j - 1], a[j]);
        }
      }
    }
  }

  T Query(int l, int r) const {
    assert(0 <= l && l < r && r <= n);
    if (r - l == 1) {
      return mat[0][l];
    }
    int p = bit_width(unsigned(l ^ (r - 1))) - 1;
    return func(mat[p][l], mat[p][r - 1]);
  }
};