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
  int st;
  int fin;
  T flow;

  flow_graph(int _n, int _st, int _fin) : n(_n), st(_st), fin(_fin) {
    assert(0 <= st && st < n && 0 <= fin && fin < n && st != fin);
    g.resize(n);
    flow = 0;
  }

  void clear_flow() {
    for (const edge &e : edges) {
      e.f = 0;
    }
    flow = 0;
  }
   
  int add(int from, int to, T forward_cap, T backward_cap) {
    assert(0 <= from && from < n && 0 <= to && to < n);
    int id = (int) edges.size();
    g[from].push_back(id);
    edges.push_back({from, to, forward_cap, 0});
    g[to].push_back(id + 1);
    edges.push_back({to, from, backward_cap, 0});
    return id;
  }
};
