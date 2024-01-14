template <typename T>
vector<int> suffix_array(int n, const T &s, int char_bound) {
  vector<int> a(n);
  if (n == 0) {
    return a;
  }
  if (char_bound != -1) {
    vector<int> aux(char_bound, 0);
    for (int i = 0; i < n; i++) {
      aux[s[i]]++;
    }
    int sum = 0;
    for (int i = 0; i < char_bound; i++) {
      int add = aux[i];
      aux[i] = sum;
      sum += add;
    }
    for (int i = 0; i < n; i++) {
      a[aux[s[i]]++] = i;
    }
  } else {
    iota(a.begin(), a.end(), 0);
    sort(a.begin(), a.end(), [&s](int i, int j) { return s[i] < s[j]; });
  }
  vector<int> sorted_by_second(n);
  vector<int> ptr_group(n);
  vector<int> new_group(n);
  vector<int> group(n);
  group[a[0]] = 0;
  for (int i = 1; i < n; i++) {
    group[a[i]] = group[a[i - 1]] + (!(s[a[i]] == s[a[i - 1]]));
  }
  int cnt = group[a[n - 1]] + 1;
  int step = 1;
  while (cnt < n) {
    int at = 0;
    for (int i = n - step; i < n; i++) {
      sorted_by_second[at++] = i;
    }
    for (int i = 0; i < n; i++) {
      if (a[i] - step >= 0) {
        sorted_by_second[at++] = a[i] - step;
      }
    }
    for (int i = n - 1; i >= 0; i--) {
      ptr_group[group[a[i]]] = i;
    }
    for (int i = 0; i < n; i++) {
      int x = sorted_by_second[i];
      a[ptr_group[group[x]]++] = x;
    }
    new_group[a[0]] = 0;
    for (int i = 1; i < n; i++) {
      if (group[a[i]] != group[a[i - 1]]) {
        new_group[a[i]] = new_group[a[i - 1]] + 1;
      } else {
        int pre = (a[i - 1] + step >= n ? -1 : group[a[i - 1] + step]);
        int cur = (a[i] + step >= n ? -1 : group[a[i] + step]);
        new_group[a[i]] = new_group[a[i - 1]] + (pre != cur);
      }
    }
    swap(group, new_group);
    cnt = group[a[n - 1]] + 1;
    step <<= 1;
  }
  return a;
}

template <typename T>
vector<int> suffix_array(const T &s, int char_bound) {
  return suffix_array((int) s.size(), s, char_bound);
}

template <typename T>
vector<int> build_lcp(int n, const T &s, const vector<int> &sa) {
  assert((int) sa.size() == n);
  vector<int> pos(n);
  for (int i = 0; i < n; i++) {
    pos[sa[i]] = i;
  }
  vector<int> lcp(max(n - 1, 0));
  int k = 0;
  for (int i = 0; i < n; i++) {
    k = max(k - 1, 0);
    if (pos[i] == n - 1) {
      k = 0;
    } else {
      int j = sa[pos[i] + 1];
      while (i + k < n && j + k < n && s[i + k] == s[j + k]) {
        k++;
      }
      lcp[pos[i]] = k;
    }
  }
  return lcp;
}

template <typename T>
vector<int> build_lcp(const T &s, const vector<int> &sa) {
  return build_lcp((int) s.size(), s, sa);
}