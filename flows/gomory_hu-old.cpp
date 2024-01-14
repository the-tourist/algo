template <typename T>
forest<T> gomory_hu(const undigraph<T> &g) {
  int n = g.n;
  if (n == 1) {
    return forest<T>(n);
  }
  flow_graph<T> fg(n, 0, 1);
  for (auto &e : g.edges) {
    fg.add(e.from, e.to, e.cost, e.cost);
  }
  vector<vector<int>> dist(n, vector<int>(n, numeric_limits<T>::max()));
  function<void(vector<int>)> dfs = [&g, &n, &fg, &dist, &dfs](vector<int> group) {
    int sz = group.size();
    if (sz == 1) {
      return;
    }
    fg.clear_flow();
    fg.st = group[0];
    fg.fin = group[1];
    T flow = fg.max_flow();
    vector<bool> cut = fg.min_cut();
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        if (cut[i] != cut[j]) {
          dist[i][j] = min(dist[i][j], flow);
        }
      }
    }
    vector<int> new_groups[2];
    for (int v : group) {
      new_groups[(int) cut[v]].push_back(v);
    }
    for (int id = 0; id < 2; id++) {
      dfs(new_groups[id]);
    }
  };
  vector<int> group(n);
  iota(group.begin(), group.end(), 0);
  dfs(group);
  undigraph<T> mg(n);
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      mg.add(i, j, -dist[i][j]);
    }
  }
  T foo;
  vector<int> ids = mst(mg, foo);
  forest<T> ret(n);
  for (int id : ids) {
    auto &e = mg.edges[id];
    ret.add(e.from, e.to, -e.cost);
  }
  return ret;
  // don't be lazy next time!
  // implement a proper gomory-hu tree
}
