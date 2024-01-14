template <typename T>
class dfs_digraph : public digraph<T> {
 public:
  using digraph<T>::edges;
  using digraph<T>::g;
  using digraph<T>::n;

  vector<int> pv;
  vector<int> pe;
  vector<int> order;
  vector<int> pos;
  vector<int> end;
  vector<int> sz;
  vector<int> root;
  vector<int> depth;
  vector<T> dist;

  dfs_digraph(int _n) : digraph<T>(_n) {
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
    dist.clear();
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
    dist = vector<T>(n);
  }

 private:
  void do_dfs(int v) {
    pos[v] = (int) order.size();
    order.push_back(v);
    sz[v] = 1;
    for (int id : g[v]) {
      if (id == pe[v]) {
        continue;
      }
      auto &e = edges[id];
      int to = e.from ^ e.to ^ v;
      // well, this is controversial...
      if (depth[to] != -1) {
        continue;
      }
      depth[to] = depth[v] + 1;
      dist[to] = dist[v] + e.cost;
      pv[to] = v;
      pe[to] = id;
      root[to] = (root[v] != -1 ? root[v] : to);
      do_dfs(to);
      sz[v] += sz[to];
    }
    end[v] = (int) order.size() - 1;
  }

  void do_dfs_from(int v) {
    depth[v] = 0;
    dist[v] = T{};
    root[v] = v;
    pv[v] = pe[v] = -1;
    do_dfs(v);
  }

 public:
  int dfs_one_unsafe(int v) {
    // run init() before this
    // then run this with the required v's
    do_dfs_from(v);
    return v;
  }

  int dfs(int v) {
    init();
    do_dfs_from(v);
//    assert((int) order.size() == n);
    return v;
  }

  void dfs_many(const vector<int> &roots) {
    init();
    for (int v : roots) {
      if (depth[v] == -1) {
        do_dfs_from(v);
      }
    }
//    assert((int) order.size() == n);
  }

  vector<int> dfs_all() {
    init();
    vector<int> roots;
    for (int v = 0; v < n; v++) {
      if (depth[v] == -1) {
        roots.push_back(v);
        do_dfs_from(v);
      }
    }
    assert((int) order.size() == n);
    return roots;
  }
};
