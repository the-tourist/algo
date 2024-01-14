#include <bits/extc++.h>

template <typename T, typename C>
class MCMF {
 public:
  static constexpr T eps = (T) 1e-9;

  struct edge {
    int from;
    int to;
    T c;
    T f;
    C cost;
  };

  int n;
  vector<vector<int>> g;
  vector<edge> edges;
  vector<C> d;
  vector<C> pot;
  __gnu_pbds::priority_queue<pair<C, int>> q;
  vector<typename decltype(q)::point_iterator> its;
  vector<int> pe;
  const C INF_C = numeric_limits<C>::max() / 2;

  explicit MCMF(int n_) : n(n_), g(n), d(n), pot(n, 0), its(n), pe(n) {}

  int add(int from, int to, T forward_cap, T backward_cap, C edge_cost) {
    assert(0 <= from && from < n && 0 <= to && to < n);
    assert(forward_cap >= 0 && backward_cap >= 0);
    int id = static_cast<int>(edges.size());
    g[from].push_back(id);
    edges.push_back({from, to, forward_cap, 0, edge_cost});
    g[to].push_back(id + 1);
    edges.push_back({to, from, backward_cap, 0, -edge_cost});
    return id;
  }

  void expath(int st) {
    fill(d.begin(), d.end(), INF_C);
    q.clear();
    fill(its.begin(), its.end(), q.end());
    its[st] = q.push({pot[st], st});
    d[st] = 0;
    while (!q.empty()) {
      int i = q.top().second;
      q.pop();
      its[i] = q.end();
      for (int id : g[i]) {
        const edge &e = edges[id];
        int j = e.to;
        if (e.c - e.f > eps && d[i] + e.cost < d[j]) {
          d[j] = d[i] + e.cost;
          pe[j] = id;
          if (its[j] == q.end()) {
            its[j] = q.push({pot[j] - d[j], j});
          } else {
            q.modify(its[j], {pot[j] - d[j], j});
          }
        }
      }
    }
    swap(d, pot);
  }
   
  pair<T, C> max_flow_min_cost(int st, int fin) {
    T flow = 0;
    C cost = 0;
    bool ok = true;
    for (auto& e : edges) {
      if (e.c - e.f > eps && e.cost + pot[e.from] - pot[e.to] < 0) {
        ok = false;
        break;
      }
    }
    if (ok) {
      expath(st);
    } else {
      vector<int> deg(n, 0);
      for (int i = 0; i < n; i++) {
        for (int eid : g[i]) {
          auto& e = edges[eid];
          if (e.c - e.f > eps) {
            deg[e.to] += 1;
          }
        }
      }
      vector<int> que;
      for (int i = 0; i < n; i++) {
        if (deg[i] == 0) {
          que.push_back(i);
        }
      }
      for (int b = 0; b < (int) que.size(); b++) {
        for (int eid : g[que[b]]) {
          auto& e = edges[eid];
          if (e.c - e.f > eps) {
            deg[e.to] -= 1;
            if (deg[e.to] == 0) {
              que.push_back(e.to);
            }
          }
        }
      }
      fill(pot.begin(), pot.end(), INF_C);
      pot[st] = 0;
      if (static_cast<int>(que.size()) == n) {
        for (int v : que) {
          if (pot[v] < INF_C) {
            for (int eid : g[v]) {
              auto& e = edges[eid];
              if (e.c - e.f > eps) {
                if (pot[v] + e.cost < pot[e.to]) {
                  pot[e.to] = pot[v] + e.cost;
                  pe[e.to] = eid;
                }
              }
            }
          }
        }
      } else {
        que.assign(1, st);
        vector<bool> in_queue(n, false);
        in_queue[st] = true;
        for (int b = 0; b < (int) que.size(); b++) {
          int i = que[b];
          in_queue[i] = false;
          for (int id : g[i]) {
            const edge &e = edges[id];
            if (e.c - e.f > eps && pot[i] + e.cost < pot[e.to]) {
              pot[e.to] = pot[i] + e.cost;
              pe[e.to] = id;
              if (!in_queue[e.to]) {
                que.push_back(e.to);
                in_queue[e.to] = true;
              }
            }
          }
        }
      }
    }
    while (pot[fin] < INF_C) {
      T push = numeric_limits<T>::max();
      int v = fin;
      while (v != st) {
        const edge &e = edges[pe[v]];
        push = min(push, e.c - e.f);
        v = e.from;
      }
      v = fin;
      while (v != st) {
        edge &e = edges[pe[v]];
        e.f += push;
        edge &back = edges[pe[v] ^ 1];
        back.f -= push;
        v = e.from;
      }
      flow += push;
      cost += push * pot[fin];
      expath(st);
    }
    return {flow, cost};
  }
};
