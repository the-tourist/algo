template <typename Info, typename Tag>
class LazySegmentTree {
 public:
  int n;
  vector<Info> infos;
  vector<Tag> tags;
  seg_tree::in_order_layout layout;

  void Apply(seg_tree::point a, const Tag& t) {
    auto [l, r] = layout.get_node_bounds(a);
    if (!t.ApplyTo(infos[a], l, r)) {
      assert(a < n);
      DowndateNode(a);
      Apply(a.c(0), t);
      Apply(a.c(1), t);
      UpdateNode(a);
      return;
    }
    if (a < n) {
      t.ApplyTo(tags[a]);
    }
  }

  void DowndateNode(seg_tree::point a) {
    if (!tags[a].Empty()) {
      Apply(a.c(0), tags[a]);
      Apply(a.c(1), tags[a]);
      tags[a] = Tag();
    }
  }

  void UpdateNode(seg_tree::point a) {
    infos[a] = infos[a.c(0)].Unite(infos[a.c(1)]);
  }

  LazySegmentTree() : LazySegmentTree(0) {}
  LazySegmentTree(int n_) : LazySegmentTree(vector<Info>(n_)) {}
  LazySegmentTree(const vector<Info>& a) : n(int(a.size())) {
    infos.resize(2 * n);
    tags.resize(n);
    layout = seg_tree::in_order_layout(n);
    for (int i = 0; i < n; i++) {
      infos[layout.get_point(i)] = a[i];
    }
    for (int i = n - 1; i >= 1; i--) {
      UpdateNode(seg_tree::point(i));
    }
  }

  void Modify(int l, int r, const Tag& t) {
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

  void Set(int p, const Info& v) {
    auto pt = layout.get_point(p);
    pt.for_parents_down([&](seg_tree::point a) {
      DowndateNode(a);
    });
    infos[pt] = v;
    pt.for_parents_up([&](seg_tree::point a) {
      UpdateNode(a);
    });
  }

  Info Query(int l, int r) {
    auto rng = layout.get_range(l, r);
    rng.for_parents_down([&](seg_tree::point a) {
      DowndateNode(a);
    });
    Info res;
    rng.for_each_l_to_r([&](seg_tree::point a) {
      res = res.Unite(infos[a]);
    });
    return res;
  }

  Info Get(int p) {
    auto pt = layout.get_point(p);
    pt.for_parents_down([&](seg_tree::point a) {
      DowndateNode(a);
    });
    return infos[pt];
  }

  template<typename F>
  int MaxRight(int l, F f) {
    auto rng = layout.get_range(l, n);
    rng.for_parents_down([&](seg_tree::point a) {
      DowndateNode(a);
    });
    int res = n;
    Info sum;
    rng.for_each_l_to_r([&](seg_tree::point a) {
      if (res != n) {
        return;
      }
      auto new_sum = sum.Unite(infos[a]);
      if (f(new_sum)) {
        sum = new_sum;
        return;
      }
      while (a < n) {
        DowndateNode(a);
        new_sum = sum.Unite(infos[a.c(0)]);
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
    Info sum;
    rng.for_each_r_to_l([&](seg_tree::point a) {
      if (res != 0) {
        return;
      }
      auto new_sum = infos[a].Unite(sum);
      if (f(new_sum)) {
        sum = new_sum;
        return;
      }
      while (a < n) {
        DowndateNode(a);
        new_sum = infos[a.c(1)].Unite(sum);
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
