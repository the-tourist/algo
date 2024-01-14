template <typename T>
class hungarian {
 public:
  static const int MAX_N = ... + 1;

  int n;
  int m;
  T a[MAX_N][MAX_N];
  T u[MAX_N];
  T v[MAX_N];
  int pa[MAX_N];
  int pb[MAX_N];
  int way[MAX_N];
  T minv[MAX_N];
  bool used[MAX_N];
  T inf;

  hungarian(int _n, int _m) : n(_n), m(_m) {
    assert(n <= m);
    T zero = T{};
    fill(u, u + n + 1, zero);
    fill(v, v + m + 1, zero);
    fill(pa, pa + n + 1, -1);
    fill(pb, pb + m + 1, -1);
    inf = numeric_limits<T>::max();
  }

  inline void add_row(int i) {
    fill(minv, minv + m + 1, inf);
    fill(used, used + m + 1, false);
    pb[m] = i;
    pa[i] = m;
    int j0 = m;
    do {
      used[j0] = true;
      int i0 = pb[j0];
      T delta = inf;
      int j1 = -1;
      for (int j = 0; j < m; j++) {
        if (!used[j]) {
          T cur = a[i0][j] - u[i0] - v[j];
          if (cur < minv[j]) {
            minv[j] = cur;
            way[j] = j0;
          }
          if (minv[j] < delta) {
            delta = minv[j];
            j1 = j;
          }
        }
      }
      for (int j = 0; j <= m; j++) {
        if (used[j]) {
          u[pb[j]] += delta;
          v[j] -= delta;
        } else {
          minv[j] -= delta;
        }
      }
      j0 = j1;
    } while (pb[j0] != -1);
    do {
      int j1 = way[j0];
      pb[j0] = pb[j1];
      pa[pb[j0]] = j0;
      j0 = j1;
    } while (j0 != m);
  }

  inline T current_score() {
    return -v[m];
  }

  inline T solve() {
    for (int i = 0; i < n; i++) {
      add_row(i);
    }
    return current_score();
  }
};
