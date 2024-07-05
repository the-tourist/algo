template <typename T>
class hld_forest_old : public lca_forest<T> {
 public:
  using lca_forest<T>::edges;
  using lca_forest<T>::g;
  using lca_forest<T>::n;
  using lca_forest<T>::pv;
  using lca_forest<T>::sz;
  using lca_forest<T>::pos;
  using lca_forest<T>::order;
  using lca_forest<T>::depth;
  using lca_forest<T>::dfs;
  using lca_forest<T>::dfs_all;
  using lca_forest<T>::lca;
  using lca_forest<T>::build_lca;

  vector<int> head;
  vector<int> visited;

  hld_forest_old(int _n) : lca_forest<T>(_n) {
    visited.resize(n);
  }

  void build_hld(const vector<int> &vs) {
    for (int tries = 0; tries < 2; tries++) {
      if (vs.empty()) {
        dfs_all();
      } else {
        order.clear();
        for (int v : vs) {
          dfs(v, false);
        }
        assert((int) order.size() == n);
      }
      if (tries == 1) {
        break;
      }
      for (int i = 0; i < n; i++) {
        if (g[i].empty()) {
          continue;
        }
        int best = -1, bid = 0;
        for (int j = 0; j < (int) g[i].size(); j++) {
          int id = g[i][j];
          int v = edges[id].from ^ edges[id].to ^ i;
          if (pv[v] != i) {
            continue;
          }
          if (sz[v] > best) {
            best = sz[v];
            bid = j;
          }
        }
        swap(g[i][0], g[i][bid]);
      }
    }
    build_lca();
    head.resize(n);
    for (int i = 0; i < n; i++) {
      head[i] = i;
    }
    for (int i = 0; i < n - 1; i++) {
      int x = order[i];
      int y = order[i + 1];
      if (pv[y] == x) {
        head[y] = head[x];
      }
    }
  }

  void build_hld(int v) {
    build_hld(vector<int>(1, v));
  }

  void build_hld_all() {
    build_hld(vector<int>());
  }

  bool apply_on_path(int x, int y, bool with_lca, function<void(int,int,bool)> f) {
    // f(x, y, up): up -- whether this part of the path goes up
    assert(!head.empty());
    int z = lca(x, y);
    if (z == -1) {
      return false;
    }
    {
      int v = x;
      while (v != z) {
        if (depth[head[v]] <= depth[z]) {
          f(pos[z] + 1, pos[v], true);
          break;
        }
        f(pos[head[v]], pos[v], true);
        v = pv[head[v]];
      }
    }
    if (with_lca) {
      f(pos[z], pos[z], false);
    }
    {
      int v = y;
      int cnt_visited = 0;
      while (v != z) {
        if (depth[head[v]] <= depth[z]) {
          f(pos[z] + 1, pos[v], false);
          break;
        }
        visited[cnt_visited++] = v;
        v = pv[head[v]];
      }
      for (int at = cnt_visited - 1; at >= 0; at--) {
        v = visited[at];
        f(pos[head[v]], pos[v], false);
      }
    }
    return true;
  }
};
