template <typename Info>
class SimpleSegmentTree {
 public:
  int n;
  vector<Info> infos;
  seg_tree::in_order_layout layout;

  void UpdateNode(seg_tree::point a) {
    infos[a] = infos[a.c(0)].Unite(infos[a.c(1)]);
  }
  
  SimpleSegmentTree(int n_) : SimpleSegmentTree(vector<Info>(n_)) {}

  SimpleSegmentTree(const vector<Info>& a) : n(int(a.size())) {
    assert(n > 0);
    infos.resize(2 * n);
    layout = seg_tree::in_order_layout(n);
    for (int i = 0; i < n; i++) {
      infos[layout.get_point(i)] = a[i];
    }
    for (int i = n - 1; i >= 1; i--) {
      infos[i] = infos[2 * i].Unite(infos[2 * i + 1]);
    }
  }

  void Set(int p, const Info& v) {
    auto pt = layout.get_point(p);
    infos[pt] = v;
    pt.for_parents_up([&](seg_tree::point a) {
      UpdateNode(a);
    });
  }

  Info Query(int l, int r) {
    auto rng = layout.get_range(l, r);
    Info res;
    rng.for_each_l_to_r([&](seg_tree::point a) {
      res = res.Unite(infos[a]);
    });
    return res;
  }

  Info Get(int p) {
    auto pt = layout.get_point(p);
    return infos[pt];
  }

  template<typename F>
  int MaxRight(int l, F f) {
    auto rng = layout.get_range(l, n);
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
