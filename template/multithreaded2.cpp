/**
 *    author:  tourist
 *    created: $CURRENT_DATE.$CURRENT_MONTH.$CURRENT_YEAR $CURRENT_HOUR:$CURRENT_MINUTE:$CURRENT_SECOND
**/
#undef _GLIBCXX_DEBUG

#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "algo/debug.h"
#else
#define debug(...) 42
#endif

mutex mut;
int qq = 0;
int tt;

class Solution {
 public:
  int test_id;

  explicit Solution(int test_id_) : test_id(test_id_) {}

  ${0}

  void ReadData() {

  }

  void Solve(stringstream& out) {

    mut.lock();
    debug(++qq, tt, test_id, clock());
    mut.unlock();
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cin >> tt;
  vector<Solution> solutions;
  solutions.reserve(tt);
  for (int test_id = 0; test_id < tt; test_id++) {
    solutions.emplace_back(test_id);
    solutions.back().ReadData();
  }
  debug("input read, kicking off");
  vector<stringstream> outs(tt);
  vector<thread> threads;
  threads.reserve(tt);
  for (int test_id = 0; test_id < tt; test_id++) {
    threads.emplace_back(&Solution::Solve, &solutions[test_id], ref(outs[test_id]));
  }
  for (int test_id = 0; test_id < tt; test_id++) {
    threads[test_id].join();
  }
  for (int i = 0; i < tt; i++) {
    cout << "Case #" << i + 1 << ": " << outs[i].str();
  }
  return 0;
}

