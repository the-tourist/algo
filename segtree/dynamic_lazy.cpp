template <typename Info, typename Tag, typename Index = int, bool Persistent = false, bool Commutative = false>
class DynamicLazySegmentTree {
 public:
  struct Node {
    array<int, 2> c;
    Info d;
    Tag t;
  };

  Index n;
  vector<Node> nodes;

  DynamicLazySegmentTree(Index n_ = 0, int q = -1) : n(n_) {
    if (q >= 0) {
      nodes.reserve(2 + q * (Commutative ? 2 : 4) * bit_width(make_unsigned_t<Index>(2 * n - 1)));
    }
    nodes.resize(2);
    nodes[1] = {{-1, -1}, Info::GetDefault(0, n), Tag()};
  }

  DynamicLazySegmentTree(const vector<Info>& init, int q = -1) : n(Index(init.size())) {
    if (q >= 0) {
      nodes.reserve(2 * n + q * (Commutative ? 2 : 4) * bit_width(make_unsigned_t<Index>(2 * n - 1)));
    }
    nodes.resize(2 * n);
    auto Build = [&](auto&& self, int id, Index nl, Index nr) -> void {
      if (nr - nl == 1) {
        nodes[id] = {{-1, -1}, init[nl], Tag()};
        return;
      }
      Index mid = (nl + nr) >> 1;
      array<int, 2> c = {id + 1, id + 2 * (mid - nl)};
      self(self, c[0], nl, mid);
      self(self, c[1], mid, nr);
      nodes[id] = {c, nodes[c[0]].d.Unite(nodes[c[1]].d), Tag()};
    };
    Build(Build, 1, 0, n);
  }

  void RefreshNode(int& id, Index nl, Index nr) {
    if (id == -1) {
      nodes.push_back({{-1, -1}, Info::GetDefault(nl, nr), Tag()});
      id = int(nodes.size()) - 1;
    } else {
      if (Persistent) {
        nodes.push_back(nodes[id]);
        id = int(nodes.size()) - 1;
      }
    }
  }

  int ModifyImpl(Index l, Index r, const Tag& t, int id, Index nl, Index nr, Tag above) {
    RefreshNode(id, nl, nr);
    if (!above.Empty()) {
      above.ApplyTo(nodes[id].d, nl, nr);
      above.ApplyTo(nodes[id].t);
    }
    if (nr <= l || nl >= r) {
      return id;
    }
    if (l <= nl && nr <= r) {
      t.ApplyTo(nodes[id].d, nl, nr);
      t.ApplyTo(nodes[id].t);
      return id;
    }
    Index mid = (nl + nr) >> 1;
    if ((!Commutative && !nodes[id].t.Empty()) || l < mid) {
      int got = ModifyImpl(l, r, t, nodes[id].c[0], nl, mid, Commutative ? Tag() : nodes[id].t);
      nodes[id].c[0] = got;
    }
    if ((!Commutative && !nodes[id].t.Empty()) || r > mid) {
      int got = ModifyImpl(l, r, t, nodes[id].c[1], mid, nr, Commutative ? Tag() : nodes[id].t);
      nodes[id].c[1] = got;
    }
    auto lft = nodes[id].c[0] == -1 ? Info::GetDefault(nl, mid) : nodes[nodes[id].c[0]].d;
    auto rgt = nodes[id].c[1] == -1 ? Info::GetDefault(mid, nr) : nodes[nodes[id].c[1]].d;
    nodes[id].d = lft.Unite(rgt);
    if (Commutative) {
      nodes[id].t.ApplyTo(nodes[id].d, nl, nr);
    } else {
      nodes[id].t = Tag();
    }
    return id;
  }

  int Modify(Index l, Index r, const Tag& t, int root = Persistent ? -1 : 1) {
    assert(0 <= l && l <= r && r <= n && root >= 1);
    return l == r ? root : ModifyImpl(l, r, t, root, 0, n, Tag());
  }

  int SetImpl(Index p, const Info& v, int id, Index nl, Index nr, Tag above) {
    RefreshNode(id, nl, nr);
    if (!above.Empty()) {
      above.ApplyTo(nodes[id].d, nl, nr);
      above.ApplyTo(nodes[id].t);
    }
    if (p < nl || p >= nr) {
      return id;
    }
    if (nr - nl == 1) {
      nodes[id].d = v;
    } else {
      Index mid = (nl + nr) >> 1;
      int got = SetImpl(p, v, nodes[id].c[0], nl, mid, nodes[id].t);
      nodes[id].c[0] = got;
      got = SetImpl(p, v, nodes[id].c[1], mid, nr, nodes[id].t);
      nodes[id].c[1] = got;
      auto lft = nodes[nodes[id].c[0]].d;
      auto rgt = nodes[nodes[id].c[1]].d;
      nodes[id].d = lft.Unite(rgt);
      nodes[id].t = Tag();
    }
    return id;
  }

  int Set(Index p, const Info& v, int root = Persistent ? -1 : 1) {
    assert(0 <= p && p < n && root >= 1);
    return SetImpl(p, v, root, 0, n, Tag());
  }

  Info QueryImpl(Index l, Index r, int id, Index nl, Index nr, Tag t) {
    if (id == -1) {
      nl = max(l, nl);
      nr = min(r, nr);
      Info res = Info::GetDefault(nl, nr);
      if (!t.Empty()) {
        t.ApplyTo(res, nl, nr);
      }
      return res;
    }
    if (l <= nl && nr <= r) {
      Info res = nodes[id].d;
      if (!t.Empty()) {
        t.ApplyTo(res, nl, nr);
      }
      return res;
    }
    auto nt = nodes[id].t;
    if (!t.Empty()) {
      t.ApplyTo(nt);
    }
    Index mid = (nl + nr) >> 1;
    auto lft = l < mid ? QueryImpl(l, r, nodes[id].c[0], nl, mid, nt) : Info();
    auto rgt = r > mid ? QueryImpl(l, r, nodes[id].c[1], mid, nr, nt) : Info();
    return lft.Unite(rgt);
  }

  Info Query(Index l, Index r, int root = Persistent ? -1 : 1) {
    assert(0 <= l && l <= r && r <= n && root >= 1);
    return l == r ? Info() : QueryImpl(l, r, root, 0, n, Tag());
  }

  Info Get(Index p, int root = Persistent ? -1 : 1) {
    assert(0 <= p && p < n && root >= 1);
    int id = root;
    Index nl = 0;
    Index nr = n;
    Tag t;
    while (nr - nl > 1 && id != -1) {
      auto nt = nodes[id].t;
      if (!t.Empty()) {
        t.ApplyTo(nt);
      }
      t = nt;
      Index mid = (nl + nr) >> 1;
      if (p < mid) {
        id = nodes[id].c[0];
        nr = mid;
      } else {
        id = nodes[id].c[1];
        nl = mid;
      }
    }
    Info res = id == -1 ? Info::GetDefault(p, p + 1) : nodes[id].d;
    if (!t.Empty()) {
      t.ApplyTo(res, p, p + 1);
    }
    return res;
  }

  template<int N, typename F>
  Index MaxRight(array<int, N> roots, Index l, F f) {
    assert(0 <= l && l <= n);
    if (l == n) {
      return n;
    }
    Index res = n;
    array<Info, N> sums;
    for (int i = 0; i < N; i++) {
      sums[i] = Info();
    }
    array<Info, N> new_sums;
    auto Dfs = [&](auto&& self, array<int, N> v, Index nl, Index nr, array<Tag, N> tags) -> void {
      if (res != n) {
        return;
      }
      array<int, N> to;
      if (nl < l) {
        Index mid = (nl + nr) >> 1;
        for (int i = 0; i < N; i++) {
          auto nt = v[i] == -1 ? Tag() : nodes[v[i]].t;
          tags[i].ApplyTo(nt);
          tags[i] = nt;
        }
        if (l < mid) {
          for (int i = 0; i < N; i++) {
            to[i] = v[i] == -1 ? -1 : nodes[v[i]].c[0];
          }
          self(self, to, nl, mid, tags);
          if (res != n) {
            return;
          }
        }
        for (int i = 0; i < N; i++) {
          to[i] = v[i] == -1 ? -1 : nodes[v[i]].c[1];
        }
        self(self, to, mid, nr, tags);
        return;
      }
      for (int i = 0; i < N; i++) {
        auto d = v[i] == -1 ? Info::GetDefault(nl, nr) : nodes[v[i]].d;
        tags[i].ApplyTo(d, nl, nr);
        new_sums[i] = sums[i].Unite(d);
      }
      if (f(new_sums)) {
        sums = new_sums;
        return;
      }
      while (nr - nl > 1) {
        Index mid = (nl + nr) >> 1;
        for (int i = 0; i < N; i++) {
          auto nt = v[i] == -1 ? Tag() : nodes[v[i]].t;
          tags[i].ApplyTo(nt);
          tags[i] = nt;
        }
        for (int i = 0; i < N; i++) {
          auto d = v[i] == -1 || nodes[v[i]].c[0] == -1 ? Info::GetDefault(nl, mid) : nodes[nodes[v[i]].c[0]].d;
          tags[i].ApplyTo(d, nl, mid);
          new_sums[i] = sums[i].Unite(d);
        }
        if (f(new_sums)) {
          sums = new_sums;
          nl = mid;
          for (int i = 0; i < N; i++) {
            v[i] = v[i] == -1 ? -1 : nodes[v[i]].c[1];
          }
        } else {
          nr = mid;
          for (int i = 0; i < N; i++) {
            v[i] = v[i] == -1 ? -1 : nodes[v[i]].c[0];
          }
        }
      }
      res = nl;
    };
    array<Tag, N> tags;
    for (int i = 0; i < N; i++) {
      tags[i] = Tag();
    }
    Dfs(Dfs, roots, 0, n, tags);
    return res;
  }

  template<int N, typename F>
  Index MinLeft(array<int, N> roots, Index r, F f) {
    assert(0 <= r && r <= n);
    if (r == 0) {
      return 0;
    }
    Index res = 0;
    array<Info, N> sums;
    for (int i = 0; i < N; i++) {
      sums[i] = Info();
    }
    array<Info, N> new_sums;
    auto Dfs = [&](auto&& self, array<int, N> v, Index nl, Index nr, array<Tag, N> tags) -> void {
      if (res != 0) {
        return;
      }
      array<int, N> to;
      if (nr > r) {
        Index mid = (nl + nr) >> 1;
        for (int i = 0; i < N; i++) {
          auto nt = v[i] == -1 ? Tag() : nodes[v[i]].t;
          tags[i].ApplyTo(nt);
          tags[i] = nt;
        }
        if (r > mid) {
          for (int i = 0; i < N; i++) {
            to[i] = v[i] == -1 ? -1 : nodes[v[i]].c[1];
          }
          self(self, to, mid, nr, tags);
          if (res != 0) {
            return;
          }
        }
        for (int i = 0; i < N; i++) {
          to[i] = v[i] == -1 ? -1 : nodes[v[i]].c[0];
        }
        self(self, to, nl, mid, tags);
        return;
      }
      for (int i = 0; i < N; i++) {
        auto d = v[i] == -1 ? Info::GetDefault(nl, nr) : nodes[v[i]].d;
        tags[i].ApplyTo(d, nl, nr);
        new_sums[i] = d.Unite(sums[i]);
      }
      if (f(new_sums)) {
        sums = new_sums;
        return;
      }
      while (nr - nl > 1) {
        Index mid = (nl + nr) >> 1;
        for (int i = 0; i < N; i++) {
          auto nt = v[i] == -1 ? Tag() : nodes[v[i]].t;
          tags[i].ApplyTo(nt);
          tags[i] = nt;
        }
        for (int i = 0; i < N; i++) {
          auto d = v[i] == -1 || nodes[v[i]].c[1] == -1 ? Info::GetDefault(mid, nr) : nodes[nodes[v[i]].c[1]].d;
          tags[i].ApplyTo(d, mid, nr);
          new_sums[i] = d.Unite(sums[i]);
        }
        if (f(new_sums)) {
          sums = new_sums;
          nr = mid;
          for (int i = 0; i < N; i++) {
            v[i] = v[i] == -1 ? -1 : nodes[v[i]].c[0];
          }
        } else {
          nl = mid;
          for (int i = 0; i < N; i++) {
            v[i] = v[i] == -1 ? -1 : nodes[v[i]].c[1];
          }
        }
      }
      res = nr;
    };
    array<Tag, N> tags;
    for (int i = 0; i < N; i++) {
      tags[i] = Tag();
    }
    Dfs(Dfs, roots, 0, n, tags);
    return res;
  }
};
