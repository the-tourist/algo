template <typename T>
vector<vector<T>> operator*(const vector<vector<T>>& a, const vector<vector<T>>& b) {
  if (a.empty() || b.empty()) {
    return {{}};
  }
  vector<vector<T>> c(a.size(), vector<T>(b[0].size()));
  for (int i = 0; i < static_cast<int>(c.size()); i++) {
    for (int j = 0; j < static_cast<int>(c[0].size()); j++) {
      c[i][j] = 0;
      for (int k = 0; k < static_cast<int>(b.size()); k++) {
        c[i][j] += a[i][k] * b[k][j];
      }
    }
  }
  return c;
}

template <typename T>
vector<vector<T>>& operator*=(vector<vector<T>>& a, const vector<vector<T>>& b) {
  return a = a * b;
}

template <typename T, typename U>
vector<vector<T>> power(const vector<vector<T>>& a, const U& b) {
  assert(b >= 0);
  vector<U> binary;
  U bb = b;
  while (bb > 0) {
    binary.push_back(bb & 1);
    bb >>= 1;
  }
  vector<vector<T>> res(a.size(), vector<T>(a.size()));
  for (int i = 0; i < static_cast<int>(a.size()); i++) {
    res[i][i] = 1;
  }
  for (int j = (int) binary.size() - 1; j >= 0; j--) {
    res *= res;
    if (binary[j] == 1) {
      res *= a;
    }
  }
  return res;
}