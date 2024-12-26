template <typename T>
class flow_graph {
 public:
  static constexpr T eps = (T) 1e-9;

  struct edge {
    int from;
    int to;
    T c;
    T f;
  };

  vector<vector<int>> g;
  vector<edge> edges;
  int n;
  int st, fin;
  T flow;

  vector<int> ptr;
  vector<int> d;
  vector<int> q;
   
  flow_graph(int _n, int _st, int _fin) : n(_n), st(_st), fin(_fin) {
    assert(0 <= st && st < n && 0 <= fin && fin < n && st != fin);
    g.resize(n);
    ptr.resize(n);
    d.resize(n);
    q.resize(n);
    flow = 0;
  }

  void clear_flow() {
    for (const edge &e : edges) {
      e.f = 0;
    }
    flow = 0;
  }
   
  void add(int from, int to, T forward_cap, T backward_cap) {
    assert(0 <= from && from < n && 0 <= to && to < n);
    g[from].push_back((int) edges.size());
    edges.push_back({from, to, forward_cap, 0});
    g[to].push_back((int) edges.size());
    edges.push_back({to, from, backward_cap, 0});
  }

  bool expath() {
    fill(d.begin(), d.end(), -1);
    q[0] = fin;
    d[fin] = 0;
    int beg = 0, end = 1;
    while (beg < end) {
      int i = q[beg++];
      for (int id : g[i]) {
        const edge &e = edges[id];
        const edge &back = edges[id ^ 1];
        if (back.c - back.f > eps && d[e.to] == -1) {
          d[e.to] = d[i] + 1;
          if (e.to == st) {
            return true;
          }
          q[end++] = e.to;
        }
      }
    }
    return false;
  }
   
  T dfs(int v, T w) {
    if (v == fin) {
      return w;
    }
    int &j = ptr[v];
    while (j >= 0) {
      int id = g[v][j];
      const edge &e = edges[id];
      if (e.c - e.f > eps && d[e.to] == d[v] - 1) {
        T t = dfs(e.to, min(e.c - e.f, w));
        if (t > eps) {
          edges[id].f += t;
          edges[id ^ 1].f -= t;
          return t;
        }
      }
      j--;
    }
    return 0;
  }

  T max_flow() {
    while (expath()) {
      for (int i = 0; i < n; i++) {
        ptr[i] = (int) g[i].size() - 1;
      }
      T big_add = 0;
      while (true) {
        T add = dfs(st, numeric_limits<T>::max());
        if (add <= eps) {
          break;
        }
        big_add += add;
      }
      if (big_add <= eps) {
        break;
      }
      flow += big_add;
    }
    return flow;
  }

  vector<bool> min_cut() {
    max_flow();
    vector<bool> ret(n);
    for (int i = 0; i < n; i++) {
      ret[i] = (d[i] != -1);
    }
    return ret;
  }

  // Maximum flow / minimum cut, Dinic's algorithm
  // Usage:
  // 1) flow_graph<T> g(n, start, finish); [T == int / int64_t / double]
  // 2) g.add(from, to, forward_cap, backward_cap);
  // 3) cout << g.max_flow() << endl;
  // 4) vector<bool> cut = g.min_cut();
  //    for (auto &e : g.edges)
  //      if (cut[e.from] != cut[e.to]) ; // edge e = (e.from -> e.to) is cut
};
