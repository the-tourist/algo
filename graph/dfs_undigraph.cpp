template <typename T>
class dfs_undigraph : public undigraph<T> {
 public:
  using undigraph<T>::edges;
  using undigraph<T>::g;
  using undigraph<T>::n;

  vector<int> pv;
  vector<int> pe;
  vector<int> order;
  vector<int> pos;
  vector<int> end;
  vector<int> sz;
  vector<int> root;
  vector<int> depth;
  vector<int> min_depth;
  vector<T> dist;
  vector<int> was;
  int attempt;

  dfs_undigraph(int _n) : undigraph<T>(_n) {
  }

  void init() {
    pv = vector<int>(n, -1);
    pe = vector<int>(n, -1);
    order.clear();
    pos = vector<int>(n, -1);
    end = vector<int>(n, -1);
    sz = vector<int>(n, 0);
    root = vector<int>(n, -1);
    depth = vector<int>(n, -1);
    min_depth = vector<int>(n, -1);
    dist = vector<T>(n);
    was = vector<int>(n, -1);
    attempt = 0;
  }

  void clear() {
    pv.clear();
    pe.clear();
    order.clear();
    pos.clear();
    end.clear();
    sz.clear();
    root.clear();
    depth.clear();
    min_depth.clear();
    dist.clear();
    was.clear();
  }

 private:
  void do_dfs(int v) {
    was[v] = attempt;
    pos[v] = (int) order.size();
    order.push_back(v);
    sz[v] = 1;
    min_depth[v] = depth[v];
    for (int id : g[v]) {
      if (id == pe[v]) {
        continue;
      }
      auto &e = edges[id];
      int to = e.from ^ e.to ^ v;
      if (was[to] == attempt) {
        min_depth[v] = min(min_depth[v], depth[to]);
        continue;
      }
      depth[to] = depth[v] + 1;
      dist[to] = dist[v] + e.cost;
      pv[to] = v;
      pe[to] = id;
      root[to] = (root[v] != -1 ? root[v] : to);
      do_dfs(to);
      sz[v] += sz[to];
      min_depth[v] = min(min_depth[v], min_depth[to]);
    }
    end[v] = (int) order.size() - 1;
  }

  void do_dfs_from(int v) {
    ++attempt;
    depth[v] = 0;
    dist[v] = T{};
    root[v] = v;
    pv[v] = pe[v] = -1;
    do_dfs(v);
  }

 public:
  void dfs(int v, bool clear_order = true) {
    if (pv.empty()) {
      init();
    } else {
      if (clear_order) {
        order.clear();
      }
    }
    do_dfs_from(v);
  }

  void dfs_all() {
    init();
    for (int v = 0; v < n; v++) {
      if (depth[v] == -1) {
        do_dfs_from(v);
      }
    }
    assert((int) order.size() == n);
  }
};
