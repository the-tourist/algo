const double eps = 1e-9;

bool IsZero(double v) {
  return abs(v) < 1e-9;
}

template <typename T>
class SparseMatrix {
 public:
  int h;
  int w;
  vector<map<int, T>> rows;
  vector<map<int, T>> cols;

  SparseMatrix(int h_, int w_) : h(h_), w(w_) {
    rows.resize(h);
    cols.resize(w);
  }

  void set(int i, int j, const T& value) {
    if (IsZero(value)) {
      rows[i].erase(j);
      cols[j].erase(i);
    } else {
      rows[i][j] = value;
      cols[j][i] = value;
    }
  }

  void modify(int i, int j, const T& value) {
    if (IsZero(value)) {
      return;
    }
    auto it = rows[i].find(j);
    if (it == rows[i].end()) {
      rows[i][j] = value;
      cols[j][i] = value;
    } else {
      it->second += value;
      if (IsZero(it->second)) {
        rows[i].erase(it);
        cols[j].erase(i);
      } else {
        cols[j][i] = it->second;
      }
    }
  }

  T get(int i, int j) {
    auto it = rows[i].find(j);
    if (it == rows[i].end()) {
      return T{};
    }
    return it->second;
  }

  void transpose() {
    swap(h, w);
    swap(rows, cols);
  }
};

template <typename T>
void GaussianElimination(SparseMatrix<T>& a, int limit) {
  assert(limit <= a.w);
  int r = 0;
  for (int c = 0; c < limit; c++) {
    int mn = a.w + 1;
    int id = -1;
    for (auto& p : a.cols[c]) {
      int i = p.first;
      if (i >= r) {
        int sz = static_cast<int>(a.rows[i].size());
        if (sz < mn) {
          mn = sz;
          id = i;
        }
      }
    }
    if (id == -1) {
      continue;
    }
    if (id > r) {
      set<int> s;
      for (auto& p : a.rows[r]) {
        s.insert(p.first);
      }
      for (auto& p : a.rows[id]) {
        s.insert(p.first);
      }
      for (int j : s) {
        T tmp = a.get(r, j);
        a.set(r, j, a.get(id, j));
        a.set(id, j, -tmp);
      }
    }
    T inv_a = 1 / a.get(r, c);
    vector<int> touched_rows;
    for (auto& p : a.cols[c]) {
      int i = p.first;
      if (i > r) {
        touched_rows.push_back(i);
        T coeff = -p.second * inv_a;
        for (auto& q : a.rows[r]) {
          if (q.first != c) {
            a.modify(i, q.first, coeff * q.second);
          }
        }
      }
    }
    for (int i : touched_rows) {
      a.set(i, c, 0);
    }
    ++r;
  }
}

template <typename T>
T Determinant(SparseMatrix<T>/*&*/ a) {
  assert(a.h == a.w);
  GaussianElimination(a, a.w);
  T d{1};
  for (int i = 0; i < a.h; i++) {
    d *= a.get(i, i);
  }
  return d;
}

template <typename T>
int Rank(SparseMatrix<T>/*&*/ a) {
  GaussianElimination(a, a.w);
  int rank = 0;
  for (int i = 0; i < a.h; i++) {
    if (!a.rows[i].empty()) {
      ++rank;
    } 
  }
  return rank;
}

template <typename T>
vector<T> SolveLinearSystem(SparseMatrix<T>/*&*/ a, const vector<T>& b) {
  assert(a.h == static_cast<int>(b.size()));
  ++a.w;
  a.cols.emplace_back();
  for (int i = 0; i < a.h; i++) {
    a.set(i, a.w - 1, b[i]);
  }
  GaussianElimination(a, a.w - 1);
  vector<T> x(a.h, 0);
  for (int r = a.h - 1; r >= 0; r--) {
    int c = a.rows[r].begin()->first;
    if (c == a.w - 1) {
      return {};
    }
    x[c] = a.get(r, a.w - 1) / a.get(r, c);
    vector<int> touched_rows;
    for (auto& q : a.cols[c]) {
      int i = q.first;
      if (i < r) {
        touched_rows.push_back(i);
        a.modify(i, a.w - 1, -x[c] * q.second);
      }
    }
    for (int i : touched_rows) {
      a.set(i, c, 0);
    }
  }
  return x;
}