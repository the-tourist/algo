struct Info {
  ${0}... a = ...;

  Info Unite(const Info& b) const {
    Info res;
    ...
    return res;
  }

  static Info GetDefault([[maybe_unused]] int l, [[maybe_unused]] int r) {
    return Info();
  }
};

