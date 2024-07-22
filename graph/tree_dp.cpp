struct Data {
  ${0}... a = ...;
};
auto Unite = [&](const Data& a, const Data& b) -> Data {
  return ...;
};
auto AddVertex = [&](const Data& a, int v) -> Data {
  return ...;
};
auto MoveUp = [&](const Data& a, int v, int eid) -> Data {
  auto& e = g.edges[eid];
  return ...;
};
g.dfs(0);
vector<Data> down(g.n);
vector<Data> up(g.n);
vector<Data> dp(g.n);
{
  for (int it = g.n - 1; it >= 0; it--) {
    int i = g.order[it];
    for (int eid : g.g[i]) {
      auto& e = g.edges[eid];
      int to = e.from ^ e.to ^ i;
      if (to == g.pv[i]) {
        continue;
      }
      down[i] = Unite(down[i], MoveUp(down[to], i, eid));
    }
    down[i] = AddVertex(down[i], i);
  }
  for (int it = 0; it < g.n; it++) {
    int i = g.order[it];
    vector<int> children;
    vector<Data> vals;
    for (int eid : g.g[i]) {
      auto& e = g.edges[eid];
      int to = e.from ^ e.to ^ i;
      if (to == g.pv[i]) {
        continue;
      }
      children.push_back(to);
      vals.push_back(MoveUp(down[to], i, eid));
    }
    vector<Data> suf(vals.size() + 1);
    for (int j = int(vals.size()) - 1; j >= 0; j--) {
      suf[j] = Unite(vals[j], suf[j + 1]);
    }
    Data pref;
    if (g.pv[i] != -1) {
      pref = MoveUp(up[i], i, g.pe[i]);
    }
    for (int j = 0; j < int(vals.size()); j++) {
      up[children[j]] = AddVertex(Unite(pref, suf[j + 1]), i);
      pref = Unite(pref, vals[j]);
    }
    dp[i] = AddVertex(pref, i);
  }
}
