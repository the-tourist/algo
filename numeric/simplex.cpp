typedef long double ld;

const ld eps = 1e-8;

vector<ld> simplex(vector<vector<ld>> a) {
  int n = (int) a.size() - 1;
  int m = (int) a[0].size() - 1;
  vector<int> left(n + 1);
  vector<int> up(m + 1);
  iota(left.begin(), left.end(), m);
  iota(up.begin(), up.end(), 0);
  auto pivot = [&](int x, int y) {
    swap(left[x], up[y]);
    ld k = a[x][y];
    a[x][y] = 1;
    vector<int> pos;
    for (int j = 0; j <= m; j++) {
      a[x][j] /= k;
      if (fabs(a[x][j]) > eps) {
        pos.push_back(j);
      }
    }
    for (int i = 0; i <= n; i++) {
      if (fabs(a[i][y]) < eps || i == x) {
        continue;
      }
      k = a[i][y];
      a[i][y] = 0;
      for (int j : pos) {
        a[i][j] -= k * a[x][j];
      }
    }
  };
  while (1) {
    int x = -1;
    for (int i = 1; i <= n; i++) {
      if (a[i][0] < -eps && (x == -1 || a[i][0] < a[x][0])) {
        x = i;
      }
    }
    if (x == -1) {
      break;
    }
    int y = -1;
    for (int j = 1; j <= m; j++) {
      if (a[x][j] < -eps && (y == -1 || a[x][j] < a[x][y])) {
        y = j;
      }
    }
    if (y == -1) {
      return vector<ld>(); // infeasible
    }
    pivot(x, y);
  }
  while (1) {
    int y = -1;
    for (int j = 1; j <= m; j++) {
      if (a[0][j] > eps && (y == -1 || a[0][j] > a[0][y])) {
        y = j;
      }
    }
    if (y == -1) {
      break;
    }
    int x = -1;
    for (int i = 1; i <= n; i++) {
      if (a[i][y] > eps && (x == -1 || a[i][0] / a[i][y] < a[x][0] / a[x][y])) {
        x = i;
      }
    }
    if (x == -1) {
      return vector<ld>(); // unbounded
    }
    pivot(x, y);
  }
  vector<ld> ans(m + 1);
  for (int i = 1; i <= n; i++) {
    if (left[i] <= m) {
      ans[left[i]] = a[i][0];
    }
  }
  ans[0] = -a[0][0];
  return ans;
}
