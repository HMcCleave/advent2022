#include <any>
#include <bitset>
#include <chrono>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <initializer_list>
#include <optional>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <utility>
#include <variant>
#include <cfloat>
#include <cinttypes>
#include <climits>
#include <cstdint>
#include <limits>
#include <string>
#include <string_view>
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iterator>
#include <algorithm>
#include <execution>
#include <cfenv>
#include <cmath>
#include <complex>
#include <numeric>
#include <random>
#include <ratio>
#include <valarray>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <streambuf>
#include <regex>

using namespace std;

void do_move(stack<char> stacks[10], int count, int source, int dest) {
  for (int i = 0; i < count; ++i) {
    stacks[dest].push(stacks[source].top());
    stacks[source].pop();
  }
}

void do_move2(stack<char> stacks[10], int count, int source, int dest) {
  stack<char> t;
  for (int i = 0; i < count; ++i) {
    t.push(stacks[source].top());
    stacks[source].pop();
  }
  for (int i = 0; i < count; ++i) {
    stacks[dest].push(t.top());
    t.pop();
  }
}

void first() {
  /*
[S]                 [T] [Q]
[L]             [B] [M] [P]     [T]
[F]     [S]     [Z] [N] [S]     [R]
[Z] [R] [N]     [R] [D] [F]     [V]
[D] [Z] [H] [J] [W] [G] [W]     [G]
[B] [M] [C] [F] [H] [Z] [N] [R] [L]
[R] [B] [L] [C] [G] [J] [L] [Z] [C]
[H] [T] [Z] [S] [P] [V] [G] [M] [M]
 1   2   3   4   5   6   7   8   9


  */
  stack<char> stacks[10];
  string inits[] = {"",        "HRBDZFLS", "TBMZR",    "ZLCHNS", "SCFJ",
                    "PGHWRZB", "VJZGDNMT", "GLNWFSPQ", "MZR",    "MCLGVRT"};
  for (int i = 0; i < 10; ++i) {
    for (auto c : inits[i])
      stacks[i].push(c);
  }
  string d;
  int count, source, dest;
  while (cin >> d >> count >> d >> source >> d >> dest) {
    do_move(stacks, count, source, dest);
  }
  for (int i = 1; i < 10; ++i) {
    if (stacks[i].size())
      cout << stacks[i].top();
  }
  cout << endl;
}

void second() {
  /*
[S]                 [T] [Q]
[L]             [B] [M] [P]     [T]
[F]     [S]     [Z] [N] [S]     [R]
[Z] [R] [N]     [R] [D] [F]     [V]
[D] [Z] [H] [J] [W] [G] [W]     [G]
[B] [M] [C] [F] [H] [Z] [N] [R] [L]
[R] [B] [L] [C] [G] [J] [L] [Z] [C]
[H] [T] [Z] [S] [P] [V] [G] [M] [M]
 1   2   3   4   5   6   7   8   9


  */
  stack<char> stacks[10];
  string inits[] = {"",        "HRBDZFLS", "TBMZR",    "ZLCHNS", "SCFJ",
                    "PGHWRZB", "VJZGDNMT", "GLNWFSPQ", "MZR",    "MCLGVRT"};
  for (int i = 0; i < 10; ++i) {
    for (auto c : inits[i])
      stacks[i].push(c);
  }
  string d;
  int count, source, dest;
  while (cin >> d >> count >> d >> source >> d >> dest) {
    do_move2(stacks, count, source, dest);
  }
  for (int i = 1; i < 10; ++i) {
    if (stacks[i].size())
      cout << stacks[i].top();
  }
  cout << endl;
}

int main() {
#ifdef SECOND
  second();
#else
  first();
#endif
}