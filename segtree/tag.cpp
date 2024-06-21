struct Tag {
  ${0}... add = ...;

  bool ApplyTo(Info& a, [[maybe_unused]] int l, [[maybe_unused]] int r) const {
    ...
    return true;
  }

  void ApplyTo(Tag& t) const {
    ...
    t.add += add;
  }

  bool Empty() const {
    ...
    return add == 0;
  }
};

