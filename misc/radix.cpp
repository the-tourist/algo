namespace radix {

vector<int> p(65537);

template<typename T>
void SortShift(vector<T>& a, vector<T>& new_a, int shift) {
  assert(a.size() == new_a.size());
  int n = static_cast<int>(a.size());
  fill(p.begin(), p.end(), 0);
  for (int i = 0; i < n; i++) p[1 + ((a[i] >> shift) & 0xffff)]++;
  for (int i = 1; i <= 65536; i++) p[i] += p[i - 1];
  for (int i = 0; i < n; i++) new_a[p[(a[i] >> shift) & 0xffff]++] = a[i];
}

void Sort(vector<int32_t>& a) {
  constexpr int32_t flip = static_cast<int32_t>(1) << 31;
  for (auto& aa : a) aa ^= flip;
  vector<int32_t> b(a.size());
  SortShift(a, b, 0);
  SortShift(b, a, 16);
  for (auto& aa : a) aa ^= flip;
}

void Sort(vector<uint32_t>& a) {
  vector<uint32_t> b(a.size());
  SortShift(a, b, 0);
  SortShift(b, a, 16);
}

void Sort(vector<int64_t>& a) {
  constexpr int64_t flip = static_cast<int64_t>(1) << 63;
  for (auto& aa : a) aa ^= flip;
  vector<int64_t> b(a.size());
  SortShift(a, b, 0);
  SortShift(b, a, 16);
  SortShift(a, b, 32);
  SortShift(b, a, 48);
  for (auto& aa : a) aa ^= flip;
}

void Sort(vector<uint64_t>& a) {
  vector<uint64_t> b(a.size());
  SortShift(a, b, 0);
  SortShift(b, a, 16);
  SortShift(a, b, 32);
  SortShift(b, a, 48);
}

}  // namespace radix