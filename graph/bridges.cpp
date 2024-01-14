template <typename T>
vector<bool> find_bridges(dfs_undigraph<T> &g) {
  g.dfs_all();
  vector<bool> bridge(g.edges.size(), false);
  for (int i = 0; i < g.n; i++) {
    if (g.pv[i] != -1 && g.min_depth[i] == g.depth[i]) {
      bridge[g.pe[i]] = true;
    }
  }
  return bridge;
}
