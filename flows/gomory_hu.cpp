template <typename T>
forest<T> gomory_hu(const undigraph<T>& g) {
  int n = g.n;
  flow_graph<T> fg(n, 0, 1);
  for (auto& e : g.edges) {
    fg.add(e.from, e.to, e.cost, e.cost);
  }
  forest<T> ret(n);
  vector<int> pr(n, 0);
  for (int i = 1; i < n; i++) {
    fg.clear_flow();
    fg.st = i;
    fg.fin = pr[i];
    T flow = fg.max_flow();
    vector<bool> cut = fg.min_cut();
    for (int j = i + 1; j < n; j++) {
      if (cut[j] == cut[i] && pr[j] == pr[i]) {
        pr[j] = i;
      }
    }
    ret.add(i, pr[i], flow);
  }
  return ret;
  // can be optimized by compressing components
}
