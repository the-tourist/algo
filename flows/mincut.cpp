template <typename T>
pair<T, vector<bool>> MinCut(vector<vector<T>> g) {
  int n = static_cast<int>(g.size());
  for (int i = 0; i < n; i++) {
    assert(static_cast<int>(g[i].size()) == n);
  }
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      assert(g[i][j] == g[j][i]);
    }
  }
  vector<vector<bool>> v(n, vector<bool>(n));
  for (int i = 0; i < n; i++) {
    v[i][i] = true;
  }
  vector<T> w(n);
  vector<bool> exists(n, true);
  vector<bool> in_a(n);
  T best_cost = numeric_limits<T>::max();
  vector<bool> best_cut;
  for (int ph = 0; ph < n - 1; ph++) {
    fill(in_a.begin(), in_a.end(), false);
    fill(w.begin(), w.end(), T(0));
    int prev = -1;
    for (int it = 0; it < n - ph; it++) {
      int sel = -1;
      for (int i = 0; i < n; i++) {
        if (exists[i] && !in_a[i] && (sel == -1 || w[i] > w[sel])) {
          sel = i;
        }
      }
      if (it == n - ph - 1) {
        if (w[sel] < best_cost) {
          best_cost = w[sel];
          best_cut = v[sel];
        }
        for (int i = 0; i < n; i++) {
          v[prev][i] = v[prev][i] | v[sel][i];
          g[prev][i] += g[sel][i];
          g[i][prev] += g[i][sel];
        }
        exists[sel] = false;
        break;
      }
      in_a[sel] = true;
      for (int i = 0; i < n; i++) {
        w[i] += g[sel][i];
      }
      prev = sel;
    }
  }
  return make_pair(best_cost, best_cut);
}
