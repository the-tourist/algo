template <typename T, typename F>
class Queue {
 public:
  vector<T> pref;
  vector<pair<T, T>> suf;
  F func;

  Queue(const F& f) : func(f) {}

  bool Empty() { return pref.empty() && suf.empty(); }
  int Size() { return int(pref.size()) + int(suf.size()); }
  void Clear() { pref.clear(); suf.clear(); }

  void Push(T t) {
    if (suf.empty()) {
      suf.emplace_back(t, t);
    } else {
      suf.emplace_back(t, func(suf.back().second, t));
    }
  }

  void Pop() {
    if (!pref.empty()) {
      pref.pop_back();
      return;
    }
    assert(!suf.empty());
    if (suf.size() > 1) {
      pref.resize(suf.size() - 1);
      pref[0] = suf.back().first;
      for (int i = 1; i < int(pref.size()); i++) {
        pref[i] = func(suf[int(suf.size()) - 1 - i].first, pref[i - 1]);
      }
    }
    suf.clear();
  }

  T Get() {
    assert(!Empty());
    if (pref.empty()) {
      return suf.back().second;
    }
    if (suf.empty()) {
      return pref.back();
    }
    return func(pref.back(), suf.back().second);
  }
};

template <typename T, typename F>
Queue<T, F> MakeQueue(const F& f) {
  return Queue<T, F>(f);
}
