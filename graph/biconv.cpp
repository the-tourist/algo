template <typename T>
vector<int> find_biconv(dfs_undigraph<T> &g, int &cnt) {
  g.dfs_all();
  vector<int> vertex_comp(g.n);
  cnt = 0;
  for (int i : g.order) {
    if (g.pv[i] == -1) {
      vertex_comp[i] = -1;
      continue;
    }
    if (g.min_depth[i] >= g.depth[g.pv[i]]) {
      vertex_comp[i] = cnt++;
    } else {
      vertex_comp[i] = vertex_comp[g.pv[i]];
    }
  }
  vector<int> edge_comp(g.edges.size(), -1);
  for (int id = 0; id < (int) g.edges.size(); id++) {
    int x = g.edges[id].from;
    int y = g.edges[id].to;
    int z = (g.depth[x] > g.depth[y] ? x : y);
    edge_comp[id] = vertex_comp[z];
  }
  return edge_comp;
}
