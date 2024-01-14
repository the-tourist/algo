template <typename T>
vector<int> find_dominators(const digraph<T> &g, int root) {
  int n = g.n;
  vector<int> pos(n, -1);
  vector<int> order;
  vector<int> parent(n, -1);
  function<void(int)> dfs = [&g, &pos, &order, &parent, &dfs](int v) {
    pos[v] = (int) order.size();
    order.push_back(v);
    for (int id : g.g[v]) {
      auto &e = g.edges[id];
      int u = e.to;
      if (pos[u] == -1) {
        parent[u] = v;
        dfs(u);
      }
    }
  };
  dfs(root);
  vector<int> p(n), best(n);
  iota(p.begin(), p.end(), 0);
  iota(best.begin(), best.end(), 0);
  vector<int> sdom = pos;
  function<int(int)> find_best = [&p, &best, &sdom, &find_best](int x) {
    if (p[x] != x) {
      int u = find_best(p[x]);
      if (sdom[u] < sdom[best[x]]) {
        best[x] = u;
      }
      p[x] = p[p[x]];
    }
    if (sdom[best[p[x]]] < sdom[best[x]]) {
      best[x] = best[p[x]];
    }
    return best[x];
  };
  digraph<int> g_rev = g.reverse();
  vector<int> idom(n, -1);
  vector<int> link(n, 0);
  vector<vector<int>> bucket(n);
  for (int it = (int) order.size() - 1; it >= 0; it--) {
    int w = order[it];
    for (int id : g_rev.g[w]) {
      auto &e = g_rev.edges[id];
      int u = e.to;
      if (pos[u] != -1) {
        sdom[w] = min(sdom[w], sdom[find_best(u)]);
      }
    }
    idom[w] = order[sdom[w]];
    for (int u : bucket[w]) {
      link[u] = find_best(u);
    }
    for (int id : g.g[w]) {
      auto &e = g.edges[id];
      int u = e.to;
      if (parent[u] == w) {
        p[u] = w;
      }
    }
    bucket[order[sdom[w]]].push_back(w);
  }
  for (int it = 1; it < (int) order.size(); it++) {
    int w = order[it];
    idom[w] = idom[link[w]];
  }
  return idom;
  // idom[i] -- immediate dominator for vertex i
}
