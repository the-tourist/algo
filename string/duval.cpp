template <typename T>
int duval(int n, const T &s) {
  assert(n >= 1);
  int i = 0, ans = 0;
  while (i < n) {
    ans = i;
    int j = i + 1, k = i;
    while (j < n + n && !(s[j % n] < s[k % n])) {
      if (s[k % n] < s[j % n]) {
        k = i;
      } else {
        k++;
      }
      j++;
    }
    while (i <= k) {
      i += j - k;
    }
  }
  return ans;
  // returns 0-indexed position of the least cyclic shift
}

template <typename T>
int duval(const T &s) {
  return duval((int) s.size(), s);
}