template <typename T>
class flow_decomposition {
 public:
  const flow_graph<T> &g;

  vector<vector<int>> paths;
  vector<T> path_flows;
  vector<vector<int>> cycles;
  vector<T> cycle_flows;

  flow_decomposition(const flow_graph<T> &_g) : g(_g) {
  }

  void decompose() {
    vector<T> fs(g.edges.size());
    for (int i = 0; i < (int) g.edges.size(); i++) {
      fs[i] = g.edges[i].f;
    }
    paths.clear();
    path_flows.clear();
    cycles.clear();
    cycle_flows.clear();
    vector<int> ptr(g.n);
    for (int i = 0; i < g.n; i++) {
      ptr[i] = (int) g.g[i].size() - 1;
    }
    vector<int> was(g.n, -1);
    int start = g.st;
    for (int iter = 0; ; iter++) {
      bool found_start = false;
      while (true) {
        if (ptr[start] >= 0) {
          int id = g.g[start][ptr[start]];
          if (fs[id] > g.eps) {
            found_start = true;
            break;
          }
          ptr[start]--;
          continue;
        }
        start = (start + 1) % g.n;
        if (start == g.st) {
          break;
        }
      }
      if (!found_start) {
        break;
      }
      vector<int> path;
      bool is_cycle = false;
      int v = start;
      while (true) {
        if (v == g.fin) {
          break;
        }
        if (was[v] == iter) {
          bool found = false;
          for (int i = 0; i < (int) path.size(); i++) {
            int id = path[i];
            auto &e = g.edges[id];
            if (e.from == v) {
              path.erase(path.begin(), path.begin() + i);
              found = true;
              break;
            }
          }
          assert(found);
          is_cycle = true;
          break;
        }
        was[v] = iter;
        bool found = false;
        while (ptr[v] >= 0) {
          int id = g.g[v][ptr[v]];
          if (fs[id] > g.eps) {
            path.push_back(id);
            v = g.edges[id].to;
            found = true;
            break;
          }
          ptr[v]--;
        }
        assert(found);
      }
      T path_flow = numeric_limits<T>::max();
      for (int id : path) {
        path_flow = min(path_flow, fs[id]);
      }
      for (int id : path) {
        fs[id] -= path_flow;
        fs[id ^ 1] += path_flow;
      }
      if (is_cycle) {
        cycles.push_back(path);
        cycle_flows.push_back(path_flow);
      } else {
        paths.push_back(path);
        path_flows.push_back(path_flow);
      }
    }
    for (const T& f : fs) {
      assert(-g.eps <= f && f <= g.eps);
    }
  }
};
