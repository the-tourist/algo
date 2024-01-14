template <typename T>
vector<T> BM(vector<T> a) {
  vector<T> p = {1};
  vector<T> q = {1};
  int l = 0;
  for (int r = 1; r <= (int) a.size(); r++) {
    T delta = 0;
    for (int j = 0; j <= l; j++) {
      delta += a[r - 1 - j] * p[j];
    }
    q.insert(q.begin(), 0);
    if (delta != 0) {
      vector<T> t = p;
      if (q.size() > t.size()) {
        t.resize(q.size());
      }
      for (int i = 0; i < (int) q.size(); i++) {
        t[i] -= delta * q[i];
      }
      if (2 * l <= r - 1) {
        q = p;
        T od = 1 / delta;
        for (T& x : q) {
          x *= od;
        }
        l = r - l;
      }
      swap(p, t);
    }
  }
  assert((int) p.size() == l + 1);
//  assert(l * 2 + 30 < (int) a.size());
  reverse(p.begin(), p.end());
  return p;
}