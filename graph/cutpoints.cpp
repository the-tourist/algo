template <typename T>
vector<bool> find_cutpoints(dfs_undigraph<T> &g) {
  g.dfs_all();
  vector<bool> cutpoint(g.n, false);
  for (int i = 0; i < g.n; i++) {
    if (g.pv[i] != -1 && g.min_depth[i] >= g.depth[g.pv[i]]) {
      cutpoint[g.pv[i]] = true;
    }
  }
  vector<int> children(g.n, 0);
  for (int i = 0; i < g.n; i++) {
    if (g.pv[i] != -1) {
      children[g.pv[i]]++;
    }
  }
  for (int i = 0; i < g.n; i++) {
    if (g.pv[i] == -1 && children[i] < 2) {
      cutpoint[i] = false;
    }
  }
  return cutpoint;
}
