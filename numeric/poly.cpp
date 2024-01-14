template <typename T>
vector<T>& operator+=(vector<T>& a, const vector<T>& b) {
  if (a.size() < b.size()) {
    a.resize(b.size());
  }
  for (int i = 0; i < (int) b.size(); i++) {
    a[i] += b[i];
  }
  return a;
}

template <typename T>
vector<T> operator+(const vector<T>& a, const vector<T>& b) {
  vector<T> c = a;
  return c += b;
}

template <typename T>
vector<T>& operator-=(vector<T>& a, const vector<T>& b) {
  if (a.size() < b.size()) {
    a.resize(b.size());
  }
  for (int i = 0; i < (int) b.size(); i++) {
    a[i] -= b[i];
  }
  return a;
}

template <typename T>
vector<T> operator-(const vector<T>& a, const vector<T>& b) {
  vector<T> c = a;
  return c -= b;
}

template <typename T>
vector<T> operator-(const vector<T>& a) {
  vector<T> c = a;
  for (int i = 0; i < (int) c.size(); i++) {
    c[i] = -c[i];
  }
  return c;
}

template <typename T>
vector<T> operator*(const vector<T>& a, const vector<T>& b) {
  if (a.empty() || b.empty()) {
    return {};
  }
  vector<T> c(a.size() + b.size() - 1, 0);
  for (int i = 0; i < (int) a.size(); i++) {
    for (int j = 0; j < (int) b.size(); j++) {
      c[i + j] += a[i] * b[j];
    }
  }
  return c;
}

template <typename T>
vector<T>& operator*=(vector<T>& a, const vector<T>& b) {
  return a = a * b;
}

template <typename T>
vector<T> inverse(const vector<T>& a) {
  assert(!a.empty());
  int n = (int) a.size();
  vector<T> b = {1 / a[0]};
  while ((int) b.size() < n) {
    vector<T> a_cut(a.begin(), a.begin() + min(a.size(), b.size() << 1));
    vector<T> x = b * b * a_cut;
    b.resize(b.size() << 1);
    for (int i = (int) b.size() >> 1; i < (int) min(x.size(), b.size()); i++) {
      b[i] = -x[i];
    }
  }
  b.resize(n);
  return b;
}

template <typename T>
vector<T>& operator/=(vector<T>& a, const vector<T>& b) {
  int n = (int) a.size();
  int m = (int) b.size();
  if (n < m) {
    a.clear();
  } else {
    vector<T> d = b;
    reverse(a.begin(), a.end());
    reverse(d.begin(), d.end());
    d.resize(n - m + 1);
    a *= inverse(d);
    a.erase(a.begin() + n - m + 1, a.end());
    reverse(a.begin(), a.end());
  }
  return a;
}

template <typename T>
vector<T> operator/(const vector<T>& a, const vector<T>& b) {
  vector<T> c = a;
  return c /= b;
}

template <typename T>
vector<T>& operator%=(vector<T>& a, const vector<T>& b) {
  int n = (int) a.size();
  int m = (int) b.size();
  if (n >= m) {
    vector<T> c = (a / b) * b;
    a.resize(m - 1);
    for (int i = 0; i < m - 1; i++) {
      a[i] -= c[i];
    }
  }
  return a;
}

template <typename T>
vector<T> operator%(const vector<T>& a, const vector<T>& b) {
  vector<T> c = a;
  return c %= b;
}

template <typename T, typename U>
vector<T> power(const vector<T>& a, const U& b, const vector<T>& c) {
  assert(b >= 0);
  vector<U> binary;
  U bb = b;
  while (bb > 0) {
    binary.push_back(bb & 1);
    bb >>= 1;
  }
  vector<T> res = vector<T>{1} % c;
  for (int j = (int) binary.size() - 1; j >= 0; j--) {
    res = res * res % c;
    if (binary[j] == 1) {
      res = res * a % c;
    }
  }
  return res;
}

template <typename T>
vector<T> derivative(const vector<T>& a) {
  vector<T> c = a;
  for (int i = 0; i < (int) c.size(); i++) {
    c[i] *= i;   	
  }
  if (!c.empty()) {
    c.erase(c.begin());
  }
  return c;
}
 
template <typename T>
vector<T> primitive(const vector<T>& a) {
  vector<T> c = a;
  c.insert(c.begin(), 0);
  for (int i = 1; i < (int) c.size(); i++) {
    c[i] /= i;
  }
  return c;
}

template <typename T>
vector<T> logarithm(const vector<T>& a) {
  assert(!a.empty() && a[0] == 1);
  vector<T> u = primitive(derivative(a) * inverse(a));
  u.resize(a.size());
  return u;
}
 
template <typename T>
vector<T> exponent(const vector<T>& a) {
  assert(!a.empty() && a[0] == 0);
  int n = (int) a.size();
  vector<T> b = {1};
  while ((int) b.size() < n) {
    vector<T> x(a.begin(), a.begin() + min(a.size(), b.size() << 1));
    x[0] += 1;
    vector<T> old_b = b;
    b.resize(b.size() << 1);
    x -= logarithm(b);
    x *= old_b;
    for (int i = (int) b.size() >> 1; i < (int) min(x.size(), b.size()); i++) {
      b[i] = x[i];
    }
  }
  b.resize(n);
  return b;
}

template <typename T>
vector<T> sqrt(const vector<T>& a) {
  assert(!a.empty() && a[0] == 1);
  int n = (int) a.size();
  vector<T> b = {1};
  while ((int) b.size() < n) {
    vector<T> x(a.begin(), a.begin() + min(a.size(), b.size() << 1));
    b.resize(b.size() << 1);
    x *= inverse(b);
    T inv2 = 1 / static_cast<T>(2);
    for (int i = (int) b.size() >> 1; i < (int) min(x.size(), b.size()); i++) {
      b[i] = x[i] * inv2;
    }
  }
  b.resize(n);
  return b;
}

template <typename T>
vector<T> multiply(const vector<vector<T>>& a) {
  if (a.empty()) {
    return {0};
  }
  function<vector<T>(int, int)> mult = [&](int l, int r) {
    if (l == r) {
      return a[l];
    }
    int y = (l + r) >> 1;
    return mult(l, y) * mult(y + 1, r);
  };
  return mult(0, (int) a.size() - 1);
}

template <typename T>
T evaluate(const vector<T>& a, const T& x) {
  T res = 0;
  for (int i = (int) a.size() - 1; i >= 0; i--) {
    res = res * x + a[i];
  }
  return res;
}

template <typename T>
vector<T> evaluate(const vector<T>& a, const vector<T>& x) {
  if (x.empty()) {
    return {};
  }
  if (a.empty()) {
    return vector<T>(x.size(), 0);
  }
  int n = (int) x.size();
  vector<vector<T>> st((n << 1) - 1);
  function<void(int, int, int)> build = [&](int v, int l, int r) {
    if (l == r) {
      st[v] = vector<T>{-x[l], 1};
    } else {
      int y = (l + r) >> 1;
      int z = v + ((y - l + 1) << 1);
      build(v + 1, l, y);
      build(z, y + 1, r);
      st[v] = st[v + 1] * st[z];
    }
  };
  build(0, 0, n - 1);
  vector<T> res(n);
  function<void(int, int, int, vector<T>)> eval = [&](int v, int l, int r, vector<T> f) {
    f %= st[v];
    if ((int) f.size() < 150) {
      for (int i = l; i <= r; i++) {
        res[i] = evaluate(f, x[i]);
      }
      return;
    }
    if (l == r) {
      res[l] = f[0];
    } else {
      int y = (l + r) >> 1;
      int z = v + ((y - l + 1) << 1);
      eval(v + 1, l, y, f);
      eval(z, y + 1, r, f);
    }
  };
  eval(0, 0, n - 1, a);
  return res;
}

template <typename T>
vector<T> interpolate(const vector<T>& x, const vector<T>& y) {
  if (x.empty()) {
    return {};
  }
  assert(x.size() == y.size());
  int n = (int) x.size();
  vector<vector<T>> st((n << 1) - 1);
  function<void(int, int, int)> build = [&](int v, int l, int r) {
    if (l == r) {
      st[v] = vector<T>{-x[l], 1};
    } else {
      int w = (l + r) >> 1;
      int z = v + ((w - l + 1) << 1);
      build(v + 1, l, w);
      build(z, w + 1, r);
      st[v] = st[v + 1] * st[z];
    }
  };
  build(0, 0, n - 1);
  vector<T> m = st[0];
  vector<T> dm = derivative(m);
  vector<T> val(n);
  function<void(int, int, int, vector<T>)> eval = [&](int v, int l, int r, vector<T> f) {
    f %= st[v];
    if ((int) f.size() < 150) {
      for (int i = l; i <= r; i++) {
        val[i] = evaluate(f, x[i]);
      }
      return;
    }
    if (l == r) {
      val[l] = f[0];
    } else {
      int w = (l + r) >> 1;
      int z = v + ((w - l + 1) << 1);
      eval(v + 1, l, w, f);
      eval(z, w + 1, r, f);
    }
  };
  eval(0, 0, n - 1, dm);
  for (int i = 0; i < n; i++) {
    val[i] = y[i] / val[i];
  }
  function<vector<T>(int, int, int)> calc = [&](int v, int l, int r) {
    if (l == r) {
      return vector<T>{val[l]};
    }
    int w = (l + r) >> 1;
    int z = v + ((w - l + 1) << 1);
    return calc(v + 1, l, w) * st[z] + calc(z, w + 1, r) * st[v + 1];
  };
  return calc(0, 0, n - 1);
}

// f[i] = 1^i + 2^i + ... + up^i
template <typename T>
vector<T> faulhaber(const T& up, int n) {
  vector<T> ex(n + 1);
  T e = 1;
  for (int i = 0; i <= n; i++) {
    ex[i] = e;
    e /= i + 1;
  }
  vector<T> den = ex;
  den.erase(den.begin());
  for (auto& d : den) {
    d = -d;
  }
  vector<T> num(n);
  T p = 1;
  for (int i = 0; i < n; i++) {
    p *= up + 1;
    num[i] = ex[i + 1] * (1 - p);
  }
  vector<T> res = num * inverse(den);
  res.resize(n);
  T f = 1;
  for (int i = 0; i < n; i++) {
    res[i] *= f;
    f *= i + 1;
  }
  return res;
}

// (x + 1) * (x + 2) * ... * (x + n)
// (can be optimized with precomputed inverses)
template <typename T>
vector<T> sequence(int n) {
  if (n == 0) {
    return {1};
  }
  if (n % 2 == 1) {
    return sequence<T>(n - 1) * vector<T>{n, 1};
  }
  vector<T> c = sequence<T>(n / 2);
  vector<T> a = c;
  reverse(a.begin(), a.end());
  T f = 1;
  for (int i = n / 2 - 1; i >= 0; i--) {
    f *= n / 2 - i;
    a[i] *= f;
  }
  vector<T> b(n / 2 + 1);
  b[0] = 1;
  for (int i = 1; i <= n / 2; i++) {
    b[i] = b[i - 1] * (n / 2) / i;
  }
  vector<T> h = a * b;
  h.resize(n / 2 + 1);
  reverse(h.begin(), h.end());
  f = 1;
  for (int i = 1; i <= n / 2; i++) {
    f /= i;
    h[i] *= f;
  }
  vector<T> res = c * h;
  return res;
}

template <typename T>
class OnlineProduct {
 public:
  const vector<T> a;
  vector<T> b;
  vector<T> c;

  OnlineProduct(const vector<T>& a_) : a(a_) {}

  T add(const T& val) {
    int i = (int) b.size();
    b.push_back(val);
    if ((int) c.size() <= i) {
      c.resize(i + 1);
    }
    c[i] += a[0] * b[i];
    int z = 1;
    while ((i & (z - 1)) == z - 1 && (int) a.size() > z) {
      vector<T> a_mul(a.begin() + z, a.begin() + min(z << 1, (int) a.size()));
      vector<T> b_mul(b.end() - z, b.end());
      vector<T> c_mul = a_mul * b_mul;
      if ((int) c.size() <= i + (int) c_mul.size()) {
        c.resize(i + c_mul.size() + 1);
      }
      for (int j = 0; j < (int) c_mul.size(); j++) {
        c[i + 1 + j] += c_mul[j];
      }
      z <<= 1;
    }
    return c[i];
  }
};