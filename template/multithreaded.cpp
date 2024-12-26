/**
 *    author:  tourist
 *    created: $CURRENT_DATE.$CURRENT_MONTH.$CURRENT_YEAR $CURRENT_HOUR:$CURRENT_MINUTE:$CURRENT_SECOND
**/
#include <bits/stdc++.h>

using namespace std;

class Solution {
  public:
  int k;
  string s, w;

  void readData() {

  }

  void solve(stringstream& out) {

  }
};

const int maxThreads = 8;
const int numTests = 1000;

stringstream out[numTests];
mutex mu;
int cur, tt;
thread threads[maxThreads];

void solutionRunner() {
  while (true) {
    Solution s;
    int id;
    mu.lock();
    if (cur >= tt) {
      mu.unlock();
      return;
    }
    id = cur;
    cur++;
    s.readData();
    mu.unlock();
    s.solve(out[id]);
  }
}

using namespace std::chrono;

int64_t now() {
  milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
  return ms.count();
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  auto start = now();
  cin >> tt;
  cur = 0;
  for (int i = 0; i < maxThreads; i++) {
    threads[i] = thread(solutionRunner);
  }
  for (int i = 0; i < maxThreads; i++) {
    threads[i].join();
  }
  for (int i = 0; i < tt; i++) {
    cout << "Case #" << i + 1 << ": " << '\n';
    cout << out[i].str();
  }
  cerr << "time = " << now() - start << " ms" << endl;
  return 0;
}
