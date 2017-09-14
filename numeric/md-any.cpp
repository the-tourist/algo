const int md = @@HERE@@;

inline unsigned fasterLLMod(unsigned long long x, unsigned y) {
#if !defined(_WIN32) || defined(_WIN64)
  if (y == md) return x % md;
#endif
  unsigned xh = (unsigned) (x >> 32), xl = (unsigned) x, d, m;
  asm(
    "divl %4; \n\t"
    : "=a" (d), "=d" (m)
    : "d" (xh), "a" (xl), "r" (y)
  );
  return m;
}

inline void add(int &a, int b, const int m = md) {
  a += b;
  if (a >= m) a -= m;
}

inline void sub(int &a, int b, const int m = md) {
  a -= b;
  if (a < 0) a += m;
}

inline int mul(int a, int b, const int m = md) {
  return fasterLLMod((long long) a * b, m);
}

inline int power(int a, int b, const int m = md) {
  int res = 1;
  while (b > 0) {
    if (b & 1) {
      res = mul(res, a, m);
    }
    a = mul(a, a, m);
    b >>= 1;
  }
  return res;
}

inline int inv(int a, const int m = md) {
  a %= m;
  if (a < 0) a += m;
  int b = m, u = 0, v = 1;
  while (a) {
    int t = b / a;
    b -= t * a; swap(a, b);
    u -= t * v; swap(u, v);
  }
  assert(b == 1);
  if (u < 0) u += m;
  return u;
}