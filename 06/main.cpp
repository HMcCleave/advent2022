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

void first() {
  map<char, int> occ;
  set<char> active;
  string in;
  cin >> in;
  for (int i = 0; i < 3; ++i) {
    ++occ[in[i]];
    active.insert(in[i]);
  }


  for (int i = 3; i < in.length(); ++i) {
    ++occ[in[i]];
    active.insert(in[i]);
    if (active.size() == 4) {
      cout << i + 1 << endl;
      return;
    }
    if (0 == --occ[in[i - 3]]) {
      active.erase(in[i - 3]);
    }
  }
}

void second() {
  map<char, int> occ;
  set<char> active;
  string in;
  cin >> in;
  for (int i = 0; i < 13; ++i) {
    ++occ[in[i]];
    active.insert(in[i]);
  }

  for (int i = 13; i < in.length(); ++i) {
    ++occ[in[i]];
    active.insert(in[i]);
    if (active.size() == 14) {
      cout << i + 1 << endl;
      return;
    }
    if (0 == --occ[in[i - 13]]) {
      active.erase(in[i - 13]);
    }
  }
}

int main() {
#ifdef SECOND
  second();
#else
  first();
#endif
}