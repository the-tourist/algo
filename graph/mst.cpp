template <typename T>
vector<int> find_mst(const undigraph<T> &g, T &ans) {
  vector<int> order(g.edges.size());
  iota(order.begin(), order.end(), 0);
  sort(order.begin(), order.end(), [&g](int a, int b) {
    return g.edges[a].cost < g.edges[b].cost;
  });
  dsu d(g.n);
  vector<int> ans_list;
  ans = 0;
  for (int id : order) {
    auto &e = g.edges[id];
    if (d.get(e.from) != d.get(e.to)) {
      d.unite(e.from, e.to);
      ans_list.push_back(id);
      ans += e.cost;
    }
  }
  return ans_list;
  // returns edge ids of minimum "spanning" forest
}
