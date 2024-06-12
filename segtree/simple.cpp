class SimpleSegmentTree {
 public:
  struct Node {
    ${0}... a = ...;
  };
  
  int n;
  vector<Node> nodes;
  seg_tree::in_order_layout layout;

  Node Unite(Node a, Node b) {
    Node res;
    ...
    return res;
  }

  void UpdateNode(seg_tree::point a) {
    nodes[a] = Unite(nodes[a.c(0)], nodes[a.c(1)]);
  }
  
  SimpleSegmentTree(int n_) : SimpleSegmentTree(vector<Node>(n_)) {}

  SimpleSegmentTree(const vector<Node>& a) : n(int(a.size())) {
    assert(n > 0);
    nodes.resize(2 * n);
    layout = seg_tree::in_order_layout(n);
    for (int i = 0; i < n; i++) {
      nodes[layout.get_point(i)] = a[i];
    }
    for (int i = n - 1; i >= 1; i--) {
      nodes[i] = Unite(nodes[2 * i], nodes[2 * i + 1]);
    }
  }

  void Set(int p, Node v) {
    auto pt = layout.get_point(p);
    nodes[pt] = v;
    pt.for_parents_up([&](seg_tree::point a) {
      UpdateNode(a);
    });
  }

  Node Get(int l, int r) {
    auto rng = layout.get_range(l, r);
    Node res;
    rng.for_each_l_to_r([&](seg_tree::point a) {
      res = Unite(res, nodes[a]);
    });
    return res;
  }

  template<typename F>
  int MaxRight(int l, F f) {
    auto rng = layout.get_range(l, n);
    int res = n;
    Node sum;
    rng.for_each_l_to_r([&](seg_tree::point a) {
      if (res != n) {
        return;
      }
      auto new_sum = Unite(sum, nodes[a]);
      if (f(new_sum)) {
        sum = new_sum;
        return;
      }
      while (a < n) {
        new_sum = Unite(sum, nodes[a.c(0)]);
        if (f(new_sum)) {
          sum = new_sum;
          a = a.c(1);
        } else {
          a = a.c(0);
        }
      }
      res = layout.get_node_bounds(a)[0];
    });
    return res;
  }

  template<typename F>
  int MinLeft(int r, F f) {
    auto rng = layout.get_range(0, r);
    int res = 0;
    Node sum;
    rng.for_each_r_to_l([&](seg_tree::point a) {
      if (res != 0) {
        return;
      }
      auto new_sum = Unite(nodes[a], sum);
      if (f(new_sum)) {
        sum = new_sum;
        return;
      }
      while (a < n) {
        new_sum = Unite(nodes[a.c(1)], sum);
        if (f(new_sum)) {
          sum = new_sum;
          a = a.c(0);
        } else {
          a = a.c(1);
        }
      }
      res = layout.get_node_bounds(a)[1];
    });
    return res;
  }
};

