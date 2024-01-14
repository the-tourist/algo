template <typename T>
vector<int> duval_prefixes(int n, const T &s) {
  vector<int> z = z_function(n, s);
  vector<int> ans(n, 0);
  int i = 0, pos = 0;
  while (i < n) {
    int j = i, k = i;
    while (j < n) {
      j++;
      if (j > pos) {
        if (z[k] <= pos - k && s[z[k]] < s[k + z[k]]) {
          int shift = (pos - i) / (j - k) * (j - k);
          ans[pos] = ans[pos - shift] + shift;
        } else {
          ans[pos] = i;
        }
        pos++;
      }
      if (s[k] < s[j]) k = i; else
      if (!(s[j] < s[k])) k++; else
      else break;
    }
    while (i <= k) {
      i += j - k;
    }
  }
  return ans;
  // returns 0-indexed positions of the least cyclic shifts of all prefixes
}

template <typename T>
vector<int> duval_prefixes(const T &s) {
  return duval_prefixes((int) s.size(), s);
}