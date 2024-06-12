class LazySegmentTree {
 public:
  struct Node {
    ${0}... a = ...;
  };
  
  struct Tag {
    ... add = ...;
  };

  int n;
  vector<Node> nodes;
  vector<Tag> tags;
  seg_tree::in_order_layout layout;

  void Apply(seg_tree::point a, Tag t) {
    ...
    nodes[a].a += ...;
    if (a < n) {
      ...
      tags[a].add += t.add;
    }
  }

  Node Unite(Node a, Node b) {
    Node res;
    ...
    return res;
  }

  void DowndateNode(seg_tree::point a) {
    ...
    if (tags[a].add != 0) {
      Apply(a.c(0), tags[a]);
      Apply(a.c(1), tags[a]);
      tags[a].add = 0;
    }
  }
  
  void UpdateNode(seg_tree::point a) {
    nodes[a] = Unite(nodes[a.c(0)], nodes[a.c(1)]);
  }
  
  LazySegmentTree(int n_) : LazySegmentTree(vector<Node>(n_)) {}

  LazySegmentTree(const vector<Node>& a) : n(int(a.size())) {
    assert(n > 0);
    nodes.resize(2 * n);
    tags.resize(n);
    layout = seg_tree::in_order_layout(n);
    for (int i = 0; i < n; i++) {
      nodes[layout.get_point(i)] = a[i];
    }
    for (int i = n - 1; i >= 1; i--) {
      nodes[i] = Unite(nodes[2 * i], nodes[2 * i + 1]);
    }
  }

  void Modify(int l, int r, Tag t) {
    auto rng = layout.get_range(l, r);
    rng.for_parents_down([&](seg_tree::point a) {
      DowndateNode(a);
    });
    rng.for_each([&](seg_tree::point a) {
      Apply(a, t);
    });
    rng.for_parents_up([&](seg_tree::point a) {
      UpdateNode(a);
    });
  }

  void Set(int p, Node v) {
    auto pt = layout.get_point(p);
    pt.for_parents_down([&](seg_tree::point a) {
      DowndateNode(a);
    });
    nodes[pt] = v;
    pt.for_parents_up([&](seg_tree::point a) {
      UpdateNode(a);
    });
  }

  Node Get(int l, int r) {
    auto rng = layout.get_range(l, r);
    rng.for_parents_down([&](seg_tree::point a) {
      DowndateNode(a);
    });
    Node res;
    rng.for_each_l_to_r([&](seg_tree::point a) {
      res = Unite(res, nodes[a]);
    });
    return res;
  }

  template<typename F>
  int MaxRight(int l, F f) {
    auto rng = layout.get_range(l, n);
    rng.for_parents_down([&](seg_tree::point a) {
      DowndateNode(a);
    });
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
        DowndateNode(a);
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
    rng.for_parents_down([&](seg_tree::point a) {
      DowndateNode(a);
    });
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
        DowndateNode(a);
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

