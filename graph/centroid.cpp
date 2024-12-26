template <typename T>
vector<int> centroid_decomposition(const forest<T>& g) {
  int n = g.n;
  vector<bool> alive(n, true);
  vector<int> res;
  res.reserve(n);
  vector<int> sz(n);
  auto Dfs = [&](auto&& self, int v, int pr) -> void {
    sz[v] = 1;
    for (int eid : g.g[v]) {
      auto& e = g.edges[eid];
      int u = e.from ^ e.to ^ v;
      if (u != pr && alive[u]) {
        self(self, u, v);
        sz[v] += sz[u];
      }
    }
  };
  auto Build = [&](auto&& self, int v) -> void {
    Dfs(Dfs, v, -1);
    int c = v;
    int pr = -1;
    while (true) {
      int nxt = -1;
      for (int eid : g.g[c]) {
        auto& e = g.edges[eid];
        int u = e.from ^ e.to ^ c;
        if (u != pr && alive[u] && 2 * sz[u] > sz[v]) {
          nxt = u;
          break;
        }
      }
      if (nxt == -1) {
        break;
      }
      pr = c;
      c = nxt;
    }
    res.push_back(c);
    alive[c] = false;
    for (int eid : g.g[c]) {
      auto& e = g.edges[eid];
      int u = e.from ^ e.to ^ c;
      if (alive[u]) {
        self(self, u);
      }
    }
  };
  for (int i = 0; i < n; i++) {
    if (alive[i]) {
      Build(Build, i);
    }
  }
  return res;
}

// Usage example:

// auto seq = centroid_decomposition(g);
// vector<bool> alive(n, true);
// for (int c : seq) {
//   alive[c] = false;
//   vector<int> go;
//   for (int eid : g.g[c]) {
//     auto& e = g.edges[eid];
//     int u = e.from ^ e.to ^ c;
//     if (alive[u]) {
//       go.push_back(u);
//     }
//   }
//   vector<pair<int, int>> p;
//   for (int r : go) {
//     vector<pair<int, int>> pp;
//     auto Dfs = [&](auto&& self, int v, int pr, int mn, int mx) -> void {
//       ans += Mint(mn) * mx;
//       pp.emplace_back(mn, mx);
//       p.emplace_back(mn, mx);
//       for (int eid : g.g[v]) {
//         auto& e = g.edges[eid];
//         int u = e.from ^ e.to ^ v;
//         if (u != pr && alive[u]) {
//           self(self, u, v, min(mn, a[u]), max(mx, a[u]));
//         }
//       }
//     };
//     Dfs(Dfs, r, c, min(a[r], a[c]), max(a[r], a[c]));
//     ans -= Solve(pp);
//   }
//   ans += Solve(p);
// }

// Another usage example:

// auto seq = centroid_decomposition(g);
// vector<FenwickTree<int64_t>> fenw;
// vector<vector<int>> fenw_depth;
// vector<vector<array<int, 3>>> fenw_data(n);
// vector<int> fenw_coeff;
// int cnt = 0;
// vector<bool> alive(n, true);
// vector<int> parent(n, -1);
// for (int c : seq) {
//   alive[c] = false;
//   vector<int> go;
//   for (int eid : g.g[c]) {
//     auto& e = g.edges[eid];
//     int u = e.from ^ e.to ^ c;
//     if (alive[u]) {
//       go.push_back(u);
//       parent[u] = c;
//     }
//   }
//   vector<pair<int, int>> p;
//   p.emplace_back(0, c);
//   for (int r : go) {
//     vector<pair<int, int>> pp;
//     auto Dfs = [&](auto&& self, int v, int pr, int depth) -> void {
//       p.emplace_back(depth, v);
//       pp.emplace_back(depth, v);
//       for (int eid : g.g[v]) {
//         auto& e = g.edges[eid];
//         int u = e.from ^ e.to ^ v;
//         if (u != pr && alive[u]) {
//           self(self, u, v, depth + 1);
//         }
//       }
//     };
//     Dfs(Dfs, r, c, 1);
//     ranges::sort(pp);
//     fenw.push_back(FenwickTree<int64_t>(int(pp.size())));
//     fenw_depth.emplace_back();
//     fenw_coeff.push_back(-1);
//     for (int i = 0; i < int(pp.size()); i++) {
//       auto [depth, v] = pp[i];
//       fenw_data[v].push_back({cnt, i, depth});
//       fenw_depth[cnt].push_back(depth);
//       fenw[cnt].Modify(i, a[v]);
//     }
//     cnt += 1;
//   }
//   ranges::sort(p);
//   fenw.push_back(FenwickTree<int64_t>(int(p.size())));
//   fenw_depth.emplace_back();
//   fenw_coeff.push_back(+1);
//   for (int i = 0; i < int(p.size()); i++) {
//     auto [depth, v] = p[i];
//     fenw_data[v].push_back({cnt, i, depth});
//     fenw_depth[cnt].push_back(depth);
//     fenw[cnt].Modify(i, a[v]);
//   }
//   cnt += 1;
// }
// while (q--) {
//   int op, v;
//   cin >> op >> v;
//   if (op == 0) {
//     int x;
//     cin >> x;
//     for (auto& [id, i, depth] : fenw_data[v]) {
//       fenw[id].Modify(i, x);
//     }
//   } else {
//     int l, r;
//     cin >> l >> r;
//     int64_t ans = 0;
//     for (auto& [id, i, depth] : fenw_data[v]) {
//       int from = max(0, l - depth);
//       int to = max(0, r - depth);
//       if (from < to) {
//         int from_pos = int(ranges::lower_bound(fenw_depth[id], from) - fenw_depth[id].begin());
//         int to_pos = int(ranges::lower_bound(fenw_depth[id], to) - fenw_depth[id].begin());
//         ans += fenw_coeff[id] * (fenw[id].Query(to_pos) - fenw[id].Query(from_pos));
//       }
//     }
//     cout << ans << '\n';
//   }
// }
