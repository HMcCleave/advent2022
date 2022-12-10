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

int priority(char c) {
  if (c >= 'a' && c <= 'z')
    return c - 'a' + 1;
  return c - 'A' + 27;
}


void first() {
  int total = 0;
  string pack;
  while (cin >> pack) {
    set<int> found;
    int halfl = pack.length() / 2;
    for (int i = 0; i < halfl; ++i) {
      found.insert(priority(pack[i]));
    }
    for (int i = 0; i < halfl; ++i) {
      int p = priority(pack[halfl + i]);
      if (found.count(p)) {
        total += p;
        break;
      }
    }
  }
  cout << total << endl;
}

map<char, int> count(string a) {
  map<char, int> out;
  for (const auto& c : a) {
    ++out[c];
  }
  return out;
}

char common(string a, string b, string c) {
  auto counta = count(a);
  auto countb = count(b);
  auto countc = count(c);
  for (auto& iter : counta) {
    const auto item = iter.first;
    if (countb[item] && countc[item])
      return item;
  }
  return a[0];
}

void second() {
  int total = 0;
  string a, b, c;
  while (cin >> a >> b >> c) {
    total += priority(common(a, b, c));
  }
  cout << total << endl;
}

int main() {
#ifdef SECOND
  second();
#else
  first();
#endif
}