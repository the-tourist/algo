struct FenwickTreeNode {
  ${0}... a = ...;

  inline void operator += (FenwickTreeNode &other) {
    a = ...(a, other.a);
  }

  inline bool operator < (FenwickTreeNode &other) {
    return a < other.a;
  }
};

