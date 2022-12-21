#include "all_helpers.hpp"

using namespace std;

ifstream _cin("sample.txt");

int64_t calculate(deque<pair<int64_t, int64_t>>& code) {
  auto zero = distance(
      code.begin(),
      find_if(code.begin(), code.end(), [](auto& x) { return x.second == 0; }));
  return code[(zero + 1000) % code.size()].second +
         code[(zero + 2000) % code.size()].second +
         code[(zero + 3000) % code.size()].second;
}

ostream& operator<<(ostream& out, const deque<pair<int64_t, int64_t>>& v) {
  for (auto i : v)
    out << i.second << ' ';
  return out << endl;
}

void shuffle(deque<pair<int64_t, int64_t>>& code) {
  for (int64_t i = 0; i < code.size(); ++i) {
    auto iter = find_if(code.begin(), code.end(),
                        [i](auto& iter) { return iter.first == i; });

    const int64_t N = code.size();
    int64_t old_pos = distance(code.begin(), iter);
    int64_t V = iter->second % (N - 1);
    if (V < 0) {
      if (old_pos == 0) {
        code.push_back(code.front());
        code.pop_front();
        old_pos = N - 1;
      }
      for (int64_t o = 0; o > V; --o) {
        --old_pos;
        swap(code[old_pos], code[old_pos + 1]);
        if (old_pos == 0) {
          code.push_back(code.front());
          code.pop_front();
          old_pos = N - 1;
        }
      }
    } else if (V > 0) {
      if (old_pos == N - 1) {
        old_pos = 0;
        code.push_front(code.back());
        code.pop_back();
      }
      for (int64_t o = 0; o < V; ++o) {
        if (++old_pos == N) {
          old_pos = 1;
          code.push_front(code.back());
          code.pop_back();
        }
        swap(code[old_pos], code[old_pos - 1]);
      }
    }

    //cout << code;
  }
}


void first() {
  deque<pair<int64_t, int64_t>> code;
  int64_t c;
  while (_cin >> c) {
    code.emplace_back(code.size(), c);
  }

  shuffle(code);

  cout << calculate(code) << endl;
}

void second() {
  deque<pair<int64_t, int64_t>> code;
  int64_t c;
  while (_cin >> c) {
    code.emplace_back(code.size(), c * 811589153);
  }

  for (int i = 0; i < 10; ++i)
    shuffle(code);

  cout << calculate(code) << endl;
}

int main()
{
  auto start = chrono::high_resolution_clock::now();
  first();
  //second();
  cout << "Time elapsed: "
       << chrono::duration_cast<chrono::milliseconds>(
              chrono::high_resolution_clock::now() - start)
              .count()
       << "ms" << endl;
}