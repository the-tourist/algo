const double eps = 1e-9;

bool IsZero(double v) {
  return abs(v) < 1e-9;
}

enum GAUSS_MODE {
  DEGREE, ABS
};

template <typename T>
void GaussianElimination(vector<vector<T>>& a, int limit, GAUSS_MODE mode = DEGREE) {
  if (a.empty() || a[0].empty()) {
    return;
  }
  int h = static_cast<int>(a.size());
  int w = static_cast<int>(a[0].size());
  for (int i = 0; i < h; i++) {
    assert(w == static_cast<int>(a[i].size()));
  }
  assert(limit <= w);
  vector<int> deg(h);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      deg[i] += !IsZero(a[i][j]);
    }
  }
  int r = 0;
  for (int c = 0; c < limit; c++) {
    int id = -1;
    for (int i = r; i < h; i++) {
      if (!IsZero(a[i][c]) && (id == -1 || (mode == DEGREE && deg[i] < deg[id]) || (mode == ABS && abs(a[id][c]) < abs(a[i][c])))) {
        id = i;
      }
    }
    if (id == -1) {
      continue;
    }
    if (id > r) {
      swap(a[r], a[id]);
      swap(deg[r], deg[id]);
      for (int j = c; j < w; j++) {
        a[id][j] = -a[id][j];
      }
    }
    vector<int> nonzero;
    for (int j = c; j < w; j++) {
      if (!IsZero(a[r][j])) {
        nonzero.push_back(j);
      }
    }
    T inv_a = 1 / a[r][c];
    for (int i = r + 1; i < h; i++) {
      if (IsZero(a[i][c])) {
        continue;
      }
      T coeff = -a[i][c] * inv_a;
      for (int j : nonzero) {
        if (!IsZero(a[i][j])) deg[i]--;
        a[i][j] += coeff * a[r][j];
        if (!IsZero(a[i][j])) deg[i]++;
      }
    }
    ++r;
  }
  for (r = h - 1; r >= 0; r--) {
    for (int c = 0; c < limit; c++) {
      if (!IsZero(a[r][c])) {
        T inv_a = 1 / a[r][c];
        for (int i = r - 1; i >= 0; i--) {
          if (IsZero(a[i][c])) {
            continue;
          }
          T coeff = -a[i][c] * inv_a;
          for (int j = c; j < w; j++) {
            a[i][j] += coeff * a[r][j];
          }
        }
        break;
      }
    }
  }
}

template <typename T>
T Determinant(vector<vector<T>>/*&*/ a) {
  if (a.empty()) {
    return T{1};
  }
  assert(a.size() == a[0].size());
  GaussianElimination(a, static_cast<int>(a[0].size()));
  T d{1};
  for (int i = 0; i < a.h; i++) {
    d *= a[i][i];
  }
  return d;
}

template <typename T>
int Rank(vector<vector<T>>/*&*/ a) {
  if (a.empty()) {
    return 0;
  }
  GaussianElimination(a, static_cast<int>(a[0].size()));
  int rank = 0;
  for (int i = 0; i < static_cast<int>(a.size()); i++) {
    for (int j = 0; j < static_cast<int>(a[i].size()); j++) {
      if (!IsZero(a[i][j])) {
        ++rank;
        break;
      }
    }
  }
  return rank;
}

template <typename T>
vector<T> SolveLinearSystem(vector<vector<T>>/*&*/ a, const vector<T>& b, int w) {
  int h = static_cast<int>(a.size());
  assert(h == static_cast<int>(b.size()));
  if (h > 0) {
    assert(w == static_cast<int>(a[0].size()));
  }
  for (int i = 0; i < h; i++) {
    a[i].push_back(b[i]);
  }
  GaussianElimination(a, w);
  vector<T> x(w, 0);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (!IsZero(a[i][j])) {
        x[j] = a[i][w] / a[i][j];
        break;
      }
    }
  }
  return x;
}

template <typename T>
vector<vector<T>> Inverse(vector<vector<T>>/*&*/ a) {
  if (a.empty()) {
    return a;
  }
  int h = static_cast<int>(a.size());
  for (int i = 0; i < h; i++) {
    assert(h == static_cast<int>(a[i].size()));
  }
  for (int i = 0; i < h; i++) {
    a[i].resize(2 * h);
    a[i][i + h] = 1;
  }
  GaussianElimination(a, h);
  for (int i = 0; i < h; i++) {
    if (IsZero(a[i][i])) {
      return {{}};
    }
  }
  vector<vector<T>> b(h);
  for (int i = 0; i < h; i++) {
    b[i] = vector<T>(a[i].begin() + h, a[i].end());
    T coeff = 1 / a[i][i];
    for (int j = 0; j < h; j++) {
      b[i][j] *= coeff;
    }
  }
  return b;
}