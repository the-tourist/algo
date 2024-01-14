template <typename T>
vector<vector<int>> find_cycles(const graph<T> &g, int bound_cnt = 1 << 30, int bound_size = 1 << 30) {
  vector<int> was(g.n, -1);
  vector<int> st;
  vector<vector<int>> cycles;
  int total_size = 0;
  function<void(int, int)> dfs = [&](int v, int pe) {
    if ((int) cycles.size() >= bound_cnt || total_size >= bound_size) {
      return;
    }
    was[v] = (int) st.size();
    for (int id : g.g[v]) {
      if (id == pe) {
        continue;
      }
      auto &e = g.edges[id];
      int to = e.from ^ e.to ^ v;
      if (was[to] >= 0) {
        vector<int> cycle(1, id);
        for (int j = was[to]; j < (int) st.size(); j++) {
          cycle.push_back(st[j]);
        }
        cycles.push_back(cycle);
        total_size += (int) cycle.size();
        if ((int) cycles.size() >= bound_cnt || total_size >= bound_size) {
          was[v] = -2;
          return;
        }
        continue;
      }
      if (was[to] == -1) {
        st.push_back(id);
        dfs(to, id);
        st.pop_back();
      }
    }
    was[v] = -2;
  };
  for (int i = 0; i < g.n; i++) {
    if (was[i] == -1) {
      dfs(i, -1);
    }
  }
  return cycles;
  // cycles are given by edge ids, all cycles are simple
  // breaks after getting bound_cnt cycles or total_size >= bound_size
  // digraph: finds at least one cycle in every connected component (if not broken)
  // undigraph: finds cycle basis
}

template <typename T>
vector<int> edges_to_vertices(const graph<T> &g, const vector<int> &edge_cycle) {
  int sz = (int) edge_cycle.size();
  vector<int> vertex_cycle;
  if (sz <= 2) {
    vertex_cycle.push_back(g.edges[edge_cycle[0]].from);
    if (sz == 2) {
      vertex_cycle.push_back(g.edges[edge_cycle[0]].to);
    }
  } else {
    for (int i = 0; i < sz; i++) {
      int j = (i + 1) % sz;
      auto &e = g.edges[edge_cycle[i]];
      auto &other = g.edges[edge_cycle[j]];
      if (other.from == e.from || other.to == e.from) {
        vertex_cycle.push_back(e.to);
      } else {
        vertex_cycle.push_back(e.from);
      }
    }
  }
  return vertex_cycle;
  // only for simple cycles!
}
